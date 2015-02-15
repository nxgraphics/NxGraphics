#include "NxNetworkPreCompiled.h"


/*
#include "../nxdeps/include/Raknet/GetTime.h"
#include "../nxdeps/include/Raknet/RakSleep.h"
#include "../nxdeps/include/Raknet/RakAssert.h"
#include "../nxdeps/include/Raknet/StringTable.h"
#include "../nxdeps/include/Raknet/RakPeerInterface.h"

#include "../nxdeps/include/Raknet/BitStream.h"
#include "../nxdeps/include/Raknet/MessageIdentifiers.h"
#include "../nxdeps/include/Raknet/ReplicaManager3.h"
#include "../nxdeps/include/Raknet/NetworkIDManager.h"
#include "../nxdeps/include/Raknet/RakSleep.h"
#include "../nxdeps/include/Raknet/FormatString.h"
#include "../nxdeps/include/Raknet/StringCompressor.h"
#include "../nxdeps/include/Raknet/Rand.h"
#include "../nxdeps/include/Raknet/DependentExtensions/Ogre3DInterpDemo/TransformationHistory.h"

using namespace RakNet;

#include <ogre.h>
using namespace Ogre;

static const int DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES=250;

// Demo variables
static const int MIN_KERNELS=100;
static const int KERNELS_VARIANCE=60;
static const RakNet::TimeMS POP_COUNTDOWN_MIN_DELAY_MS=1000;
static const RakNet::TimeMS POP_COUNTDOWN_VARIANCE_MS=5000;
static const RakNet::TimeMS RESTART_TIMER_MS=14000;
static const float POSITION_VARIANCE=100.0f;
static const float PLANE_VELOCITY_VARIANCE=30.0f;
static const float UPWARD_VELOCITY_MINIMUM=35.0f;
static const float UPWARD_VELOCITY_VARIANCE=25.0f;
static const float DOWNWARD_ACCELERATION = -15.0f;

bool isServer = true;

class Popcorn;
Ogre::Entity *popcornKernel, *popcornPopped;
RakNet::RakPeerInterface *rakPeer;
DataStructures::List<Popcorn*> popcornList;
bool enableInterpolation;


class Popcorn : public Replica3
{
public:
	Popcorn() {
		// Buffer up for 3 seconds if we were to get 30 updates per second
		transformationHistory.Init(30,3000);

		position=Ogre::Vector3::ZERO;
		orientation=Ogre::Quaternion::IDENTITY;
		// Visible position is where we are interpolating at, which is behind the real position
		visiblePosition=Ogre::Vector3::ZERO;
		visibleOrientation=Ogre::Quaternion::IDENTITY;
		isKernel=true;
	}
	virtual ~Popcorn()
	{
		if (isServer)
			BroadcastDestruction();

		//app->GetSceneManager()->destroyEntity(sceneNode->getAttachedObject(0)->getName());
		//app->GetSceneManager()->getRootSceneNode()->removeAndDestroyChild(sceneNode->getName());
		popcornList.RemoveAtIndex(popcornList.GetIndexOf(this));
	}

	bool isKernel;
	Ogre::Vector3 position;
	Ogre::Quaternion orientation;
	Ogre::Quaternion rotationalVelocity;
	Ogre::Vector3 velocity;
	Ogre::SceneNode *sceneNode;
	RakNet::TimeMS popCountdown;
	Ogre::Vector3 visiblePosition;
	Ogre::Quaternion visibleOrientation;
	TransformationHistory transformationHistory;

	virtual void WriteAllocationID(RakNet::BitStream *allocationIdBitstream) const
	{
		StringTable::Instance()->EncodeString("Popcorn", 128, allocationIdBitstream);
	}
	virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3)
	{
		if (isServer)
			return QueryConstruction_ServerConstruction(destinationConnection, true );
		else
			return QueryConstruction_ClientConstruction(destinationConnection, false );
	}
	virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection){
		if (isServer)
			return QueryRemoteConstruction_ServerConstruction(sourceConnection, true);
		else
			return QueryRemoteConstruction_ClientConstruction(sourceConnection, false);
	}
	virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection){}
	virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection){return true;}
	virtual void SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection){}
	virtual bool DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection){return true;}
	virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const
	{
		if (isServer)
			return QueryActionOnPopConnection_Server(droppedConnection);
		else
			return QueryActionOnPopConnection_Client(droppedConnection);
	}
	virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection) {delete this;}
	virtual RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection)
	{
		if (isServer)
			return QuerySerialization_ServerSerializable(destinationConnection, true);
		else
			return QuerySerialization_ClientSerializable(destinationConnection, false);
	}
	virtual RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters)
	{
		// Autoserialize causes a network packet to go out when any of these member variables change.
		RakAssert(isServer==true);
		serializeParameters->outputBitstream[0].Write(isKernel);
		serializeParameters->outputBitstream[0].WriteAlignedBytes((const unsigned char*)&position,sizeof(position));
		serializeParameters->outputBitstream[0].WriteAlignedBytes((const unsigned char*)&velocity,sizeof(velocity));
		serializeParameters->outputBitstream[0].WriteAlignedBytes((const unsigned char*)&orientation,sizeof(orientation));
		return RM3SR_BROADCAST_IDENTICALLY;
	}	
	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters)
	{
		bool lastIsKernel = isKernel;

		// Doing this because we are also lagging position and orientation behind by DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES
		// Without it, the kernel would pop immediately but would not start moving
		deserializeParameters->serializationBitstream[0].Read(isKernel);
		if (isKernel==false && lastIsKernel==true)
		popCountdown=DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES;

		deserializeParameters->serializationBitstream[0].ReadAlignedBytes((unsigned char*)&position,sizeof(position));
		deserializeParameters->serializationBitstream[0].ReadAlignedBytes((unsigned char*)&velocity,sizeof(velocity));
		deserializeParameters->serializationBitstream[0].ReadAlignedBytes((unsigned char*)&orientation,sizeof(orientation));

		// Scene node starts invisible until we deserialize the intial startup data
		// This data could also have been passed in SerializeConstruction()
		sceneNode->setVisible(true,true);

		// Every time we get a network packet, we write it to the transformation history class.
		// This class, given a time in the past, can then return to us an interpolated position of where we should be in at that time
		transformationHistory.Write(position,velocity,orientation,RakNet::GetTimeMS());
	}

	virtual void SetToPopped(void)
	{
		// Change the mesh, and add some velocity.
		isKernel=false;
		if (sceneNode->getAttachedObject(0))
		{
			//app->GetSceneManager()->destroyEntity(sceneNode->getAttachedObject(0)->getName());
		}

		sceneNode->detachAllObjects();
		sceneNode->attachObject(popcornPopped->clone(FormatString("%p",this)));
		if (isServer)
		{
			velocity.x=-PLANE_VELOCITY_VARIANCE/2.0f+frandomMT()*PLANE_VELOCITY_VARIANCE;
			velocity.y=UPWARD_VELOCITY_MINIMUM+frandomMT()*UPWARD_VELOCITY_VARIANCE;
			velocity.z=-PLANE_VELOCITY_VARIANCE/2.0f+frandomMT()*PLANE_VELOCITY_VARIANCE;
		}		
	}
	
	virtual void Update(RakNet::TimeMS timeElapsedMs)
	{
		visiblePosition=position;
		visibleOrientation=orientation;

		if (isKernel==false)
		{
			if (isServer)
			{
				// Only the server is doing physics
				float timeElapsedSec = timeElapsedMs * .001f;
				position += velocity * timeElapsedSec + .5f * Ogre::Vector3(0.0f, DOWNWARD_ACCELERATION, 0.0f) * timeElapsedSec*timeElapsedSec;;
				velocity += Ogre::Vector3(0.0f, DOWNWARD_ACCELERATION, 0.0f) * timeElapsedSec;
				orientation = Quaternion::Slerp(timeElapsedSec, orientation, orientation * rotationalVelocity, true);
			}
			else
			{
				// See above - delay the pop until we start moving
				if (popCountdown <= timeElapsedMs)
				{
					SetToPopped();
					popCountdown=-1;
				}
				else
					popCountdown-=timeElapsedMs;

				// interpolate visible position, lagging behind by a small amount so where know where to update to
				if (enableInterpolation)
				{
					// Important: the first 3 parameters are in/out parameters, so set their values to the known current values before calling Read()
					// We are subtracting DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES from the current time to get an interpolated position in the past
					// Without this we wouldn't have a node to interpolate to, and wouldn't know where to go
					transformationHistory.Read(&visiblePosition, 0, &visibleOrientation, RakNet::GetTimeMS()-DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES,RakNet::GetTimeMS());
				}
			}
		}
		else
		{
			if (isServer)
			{
				if (popCountdown <= timeElapsedMs)
				{
					// Only the server controls when to pop
					SetToPopped();
				}
				else
					popCountdown-=timeElapsedMs;
			}
			
		}

		sceneNode->setPosition(visiblePosition);
		sceneNode->setOrientation(visibleOrientation);
	}


	static void ClearPopcorn()
	{
		// Destructor removes itself from this list
		while (popcornList.Size())
			delete popcornList[popcornList.Size()-1];
	}
	static Popcorn * CreateKernel(ReplicaManager3 *replicaManager3)
	{
		Popcorn *p = new Popcorn;
		// Tell the replication system about this new Replica instance.
		replicaManager3->Reference(p);
		static int count=0;
		count++;
		popcornList.Insert(p, _FILE_AND_LINE_ );

	 
		
		//p->sceneNode = app->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
		//p->sceneNode->attachObject(popcornKernel->clone(FormatString("%p",p)));

		//// Only server sets up initial positions, etc.
		//if (isServer)
		//{
		//	p->position.x=-POSITION_VARIANCE/2.0f+frandomMT()*POSITION_VARIANCE;
		//	p->position.y=0.0f;
		//	p->position.z=-POSITION_VARIANCE/2.0f+frandomMT()*POSITION_VARIANCE;
		//	p->velocity=Ogre::Vector3::ZERO;
		//	p->popCountdown=POP_COUNTDOWN_MIN_DELAY_MS + randomMT() % POP_COUNTDOWN_VARIANCE_MS;
		//	p->orientation.FromAngleAxis(Ogre::Radian(frandomMT()*6.28f), Ogre::Vector3(-1.0f+frandomMT()*2.0f,-1.0f+frandomMT()*2.0f,-1.0f+frandomMT()*2.0f).normalisedCopy());
		//	p->rotationalVelocity.FromAngleAxis(Ogre::Radian(frandomMT()*6.28f), Ogre::Vector3(-1.0f+frandomMT()*2.0f,-1.0f+frandomMT()*2.0f,-1.0f+frandomMT()*2.0f).normalisedCopy());
		//	p->visiblePosition=p->position;
		//	p->visibleOrientation=p->orientation;
		//}
		//else
		//	p->sceneNode->setVisible(false,true);

			

		 


		return p;
	}
};

// One instance of Connection_RM2 is implicitly created per connection that uses ReplicaManager2. The most important function to implement is Construct() as this creates your game objects.
// It is designed this way so you can override per-connection behavior in your own game classes
class PopcornSampleConnection : public Connection_RM3
{
public:
	PopcornSampleConnection(SystemAddress _systemAddress, RakNetGUID _guid) : Connection_RM3(_systemAddress,_guid) 
	{

	}
	virtual ~PopcornSampleConnection()
	{
	
	}

	// Callback used to create objects
	// See Connection_RM2::Construct in ReplicaManager2.h for a full explanation of each parameter
	virtual Replica3 *AllocReplica(RakNet::BitStream *allocationIdBitstream, ReplicaManager3 *replicaManager3)
	{
 
		char objectName[128];
		StringTable::Instance()->DecodeString(objectName,128,allocationIdBitstream);
		if (strcmp(objectName,"Popcorn")==0)
		{
			return Popcorn::CreateKernel(replicaManager3);
		}
 
		return 0;
	}
};

class PopcornDemoRM3 : public ReplicaManager3
{
	virtual Connection_RM3* AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const 
	{
		Log("Client Connected...");
		return new PopcornSampleConnection(systemAddress,rakNetGUID);
	}
	virtual void DeallocConnection(Connection_RM3 *connection) const 
	{
		Log("Client Disconnected...");
		delete connection;
	}
};

PopcornDemoRM3 replicaManager3;
*/


 

#include "curl.h"

namespace NxNetwork {

template<> NxNetworkManager * Singleton<NxNetworkManager>::msSingleton = 0;

NxNetworkManager::NxNetworkManager() 
{
	//initialize the log manager
	new NxNetworkLog();

	//initialize curl
	curl_global_init(CURL_GLOBAL_ALL);	

	Log("Started NxNetwork.");
}

NxNetworkManager::~NxNetworkManager()
{
	curl_global_cleanup();
}

NxNetworkManager& NxNetworkManager::getSingleton(void)
{
	assert( msSingleton );  return ( *msSingleton );
}

NxNetworkServer * NxNetworkManager::CreateServer( int Port, int MaxClients )
{
	/*
	isServer = true;
	RakNet::SocketDescriptor sd;
	sd.port= Port;

	//RakNet::NetworkIDManager networkIDManager;
	//networkIdManager.SetIsNetworkIDAuthority(isServer);

	rakPeer = RakNet::RakPeerInterface::GetInstance();
	StartupResult StartRes = rakPeer->Startup( 32, &sd, 1 );
	if( StartRes == RAKNET_STARTED )
	{
		Log("Server Started");
	}

	
	//RAKNET_STARTED,
	//RAKNET_ALREADY_STARTED,
	//INVALID_SOCKET_DESCRIPTORS,
	//INVALID_MAX_CONNECTIONS,
	//SOCKET_PORT_ALREADY_IN_USE,
	//SOCKET_FAILED_TO_BIND,
	//SOCKET_FAILED_TEST_SEND,
	//FAILED_TO_CREATE_NETWORK_THREAD,
	

	// The server should allow systems to connect. Clients do not need to unless you want to use RakVoice or for some other reason want to transmit directly between systems.
	rakPeer->SetMaximumIncomingConnections(32);

	NetworkIDManager * networkIDManager = new NetworkIDManager();
	PopcornDemoRM3 * replicaManager3 = new PopcornDemoRM3();
	replicaManager3->SetNetworkIDManager(networkIDManager);

	rakPeer->AttachPlugin(replicaManager3);
	// Automatically destroy connections, but don't create them so we have more control over when a system is considered ready to play
	replicaManager3->SetAutoManageConnections(false,true);

	// StringTable has to be called after RakPeer started, or else first call StringTable::AddRef() yourself
	StringTable::Instance()->AddString("Popcorn",false);
	*/

	return new NxNetworkServer();
}

NxNetworkClient * NxNetworkManager::CreateClient( int Ip, int Port )
{
	/*
	isServer = false;
	static const char *SERVER_IP_ADDRESS = "127.0.0.1";
	RakNet::SocketDescriptor sd;
	//sd.port= Port;

	rakPeer = RakNet::RakPeerInterface::GetInstance();

	rakPeer->Startup( 1, &sd, 30, 1);
	rakPeer->AttachPlugin(&replicaManager3);
	ConnectionAttemptResult res = rakPeer->Connect( SERVER_IP_ADDRESS, Port, 0, 0);
	if( res == CONNECTION_ATTEMPT_STARTED )
	{
		Log("Client Connected");
	}
	else
	{
		Log("Client could not Connect");
	}

	const char* message = "Hello World";
	//To all connected systems:
	rakPeer->Send((char*)message, strlen(message)+1, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	
	//rakPeer->Startup( 1,&sd,1);
	//rakPeer->SetNetworkIDManager(&networkIdManager);
	// The server should allow systems to connect. Clients do not need to unless you want to use RakVoice or for some other reason want to transmit directly between systems.
	//ConnectionAttemptResult Res = rakPeer->Connect(SERVER_IP_ADDRESS, Port,0,0);

	// StringTable has to be called after RakPeer started, or else first call StringTable::AddRef() yourself
	StringTable::Instance()->AddString("Popcorn",false);
	*/

	return new NxNetworkClient();
}


} // namespace