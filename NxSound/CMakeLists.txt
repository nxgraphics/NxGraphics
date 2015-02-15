# NXSOUND CMake build file
cmake_minimum_required(VERSION 2.8.4)

project(NxSound)

set(NxSound_LIBRARY NxSound)

set(NxSound_src
./NxSound.cpp		
./NxSound_Device.cpp		
./NxSound_Device_Input.cpp		
./NxSound_Device_Output.cpp		
./NxSound_Log.cpp		
./NxSound_Media_DataTypes.cpp		
./NxSound_OpenAl.cpp	
./NxSound_Plateform.cpp	
./NxSound_Utilities.cpp		
./NxSoundPreCompiled.cpp
./NxSound_Effect_Base.cpp
./NxSound_Effect_VSTHost.cpp
./NxSound_Effect_VSTPluginPreset.cpp
./NxSound_FFT.cpp
./NxSound_Effect_3D.cpp
./NxSound_Effect_BeatDetector.cpp
./NxSound_Effect_Chorus.cpp
./NxSound_Effect_Echo.cpp
./NxSound_Effect_Granulate.cpp
./NxSound_Effect_OggBroadCaster.cpp
./NxSound_Effect_Shift.cpp
./FileReadBase.cpp
./FileReadFlac.cpp
./FileReadMp3.cpp
./FileReadOgg.cpp
./FileReadWav.cpp
./NxSound_Media_File.cpp
./NxSound_Media_File_FLAC.cpp
./NxSound_Media_File_MP3.cpp
./NxSound_Media_File_OGG.cpp
./NxSound_Media_File_VSTi.cpp
./NxSound_Media_File_WAV.cpp
./NxSound_Media_File_Buffer.cpp
./NxSound_Media_Inst_MODAL.cpp
../nxdeps/include/speex/resample.c
../nxdeps/include/STK/src/Stk.cpp
../nxdeps/include/STK/src/ADSR.cpp
../nxdeps/include/STK/src/Asymp.cpp
../nxdeps/include/STK/src/BandedWG.cpp			
../nxdeps/include/STK/src/BeeThree.cpp			
../nxdeps/include/STK/src/BiQuad.cpp			
../nxdeps/include/STK/src/Blit.cpp			
../nxdeps/include/STK/src/BlitSaw.cpp
../nxdeps/include/STK/src/BlitSquare.cpp
../nxdeps/include/STK/src/BlowBotl.cpp
../nxdeps/include/STK/src/BlowHole.cpp			
../nxdeps/include/STK/src/Bowed.cpp			
../nxdeps/include/STK/src/Brass.cpp
../nxdeps/include/STK/src/Chorus.cpp			
../nxdeps/include/STK/src/Clarinet.cpp			
../nxdeps/include/STK/src/Delay.cpp
../nxdeps/include/STK/src/DelayA.cpp			
../nxdeps/include/STK/src/DelayL.cpp		
../nxdeps/include/STK/src/Drummer.cpp			
../nxdeps/include/STK/src/Echo.cpp
../nxdeps/include/STK/src/Envelope.cpp
../nxdeps/include/STK/src/FileLoop.cpp
../nxdeps/include/STK/src/FileRead.cpp			
../nxdeps/include/STK/src/FileWrite.cpp
../nxdeps/include/STK/src/FileWvIn.cpp			
../nxdeps/include/STK/src/FileWvOut.cpp		
../nxdeps/include/STK/src/Fir.cpp
../nxdeps/include/STK/src/Flute.cpp
../nxdeps/include/STK/src/FM.cpp
../nxdeps/include/STK/src/FMVoices.cpp			
../nxdeps/include/STK/src/FormSwep.cpp
../nxdeps/include/STK/src/Granulate.cpp
../nxdeps/include/STK/src/HevyMetl.cpp
../nxdeps/include/STK/src/Iir.cpp
../nxdeps/include/STK/src/InetWvIn.cpp		
../nxdeps/include/STK/src/InetWvOut.cpp			
../nxdeps/include/STK/src/JCRev.cpp
../nxdeps/include/STK/src/LentPitShift.cpp
../nxdeps/include/STK/src/Mandolin.cpp		
../nxdeps/include/STK/src/Mesh2D.cpp			
../nxdeps/include/STK/src/Messager.cpp			
../nxdeps/include/STK/src/MidiFileIn.cpp
../nxdeps/include/STK/src/Modal.cpp
../nxdeps/include/STK/src/ModalBar.cpp			
../nxdeps/include/STK/src/Modulate.cpp	
../nxdeps/include/STK/src/Moog.cpp	
../nxdeps/include/STK/src/Mutex.cpp			
../nxdeps/include/STK/src/Noise.cpp		
../nxdeps/include/STK/src/NRev.cpp			
../nxdeps/include/STK/src/OnePole.cpp			
../nxdeps/include/STK/src/OneZero.cpp			
../nxdeps/include/STK/src/PercFlut.cpp		
../nxdeps/include/STK/src/Phonemes.cpp			
../nxdeps/include/STK/src/PitShift.cpp			
../nxdeps/include/STK/src/Plucked.cpp			
../nxdeps/include/STK/src/PoleZero.cpp			
../nxdeps/include/STK/src/PRCRev.cpp			
../nxdeps/include/STK/src/Resonate.cpp
../nxdeps/include/STK/src/Rhodey.cpp		
../nxdeps/include/STK/src/RtAudio.cpp
../nxdeps/include/STK/src/RtMidi.cpp			
../nxdeps/include/STK/src/RtWvIn.cpp			
../nxdeps/include/STK/src/RtWvOut.cpp			
../nxdeps/include/STK/src/Sampler.cpp			
../nxdeps/include/STK/src/Saxofony.cpp			
../nxdeps/include/STK/src/Shakers.cpp
../nxdeps/include/STK/src/Simple.cpp			
../nxdeps/include/STK/src/SineWave.cpp			
../nxdeps/include/STK/src/SingWave.cpp			
../nxdeps/include/STK/src/Sitar.cpp			
../nxdeps/include/STK/src/Skini.cpp		
../nxdeps/include/STK/src/Socket.cpp			
../nxdeps/include/STK/src/Sphere.cpp			
../nxdeps/include/STK/src/StifKarp.cpp						
../nxdeps/include/STK/src/TapDelay.cpp			
../nxdeps/include/STK/src/TcpClient.cpp
../nxdeps/include/STK/src/TcpServer.cpp
../nxdeps/include/STK/src/Thread.cpp
../nxdeps/include/STK/src/TubeBell.cpp
../nxdeps/include/STK/src/Twang.cpp
../nxdeps/include/STK/src/TwoPole.cpp
../nxdeps/include/STK/src/TwoZero.cpp
../nxdeps/include/STK/src/UdpSocket.cpp	
../nxdeps/include/STK/src/Voicer.cpp
../nxdeps/include/STK/src/VoicForm.cpp
../nxdeps/include/STK/src/Whistle.cpp
../nxdeps/include/STK/src/Wurley.cpp
../nxdeps/include/STK/include/ADSR.h
../nxdeps/include/STK/include/Asymp.h
../nxdeps/include/STK/include/BandedWG.h
../nxdeps/include/STK/include/BeeThree.h
../nxdeps/include/STK/include/BiQuad.h
../nxdeps/include/STK/include/Blit.h
../nxdeps/include/STK/include/BlitSaw.h
../nxdeps/include/STK/include/BlitSquare.h
../nxdeps/include/STK/include/BlowBotl.h
../nxdeps/include/STK/include/BlowHole.h
../nxdeps/include/STK/include/Bowed.h
../nxdeps/include/STK/include/BowTable.h
../nxdeps/include/STK/include/Brass.h
../nxdeps/include/STK/include/Chorus.h
../nxdeps/include/STK/include/Clarinet.h
../nxdeps/include/STK/include/Cubic.h
../nxdeps/include/STK/include/Delay.h
../nxdeps/include/STK/include/DelayA.h
../nxdeps/include/STK/include/DelayL.h
../nxdeps/include/STK/include/Drummer.h
../nxdeps/include/STK/include/Echo.h
../nxdeps/include/STK/include/Effect.h
../nxdeps/include/STK/include/Envelope.h
../nxdeps/include/STK/include/FileLoop.h
../nxdeps/include/STK/include/FileRead.h
../nxdeps/include/STK/include/FileWrite.h
../nxdeps/include/STK/include/FileWvIn.h
../nxdeps/include/STK/include/FileWvOut.h
../nxdeps/include/STK/include/Filter.h
../nxdeps/include/STK/include/Fir.h
../nxdeps/include/STK/include/Flute.h
../nxdeps/include/STK/include/FM.h
../nxdeps/include/STK/include/FMVoices.h
../nxdeps/include/STK/include/FormSwep.h
../nxdeps/include/STK/include/Function.h
../nxdeps/include/STK/include/Generator.h
../nxdeps/include/STK/include/Granulate.h
../nxdeps/include/STK/include/HevyMetl.h
../nxdeps/include/STK/include/Iir.h
../nxdeps/include/STK/include/InetWvIn.h
../nxdeps/include/STK/include/InetWvOut.h
../nxdeps/include/STK/include/Instrmnt.h
../nxdeps/include/STK/include/JCRev.h
../nxdeps/include/STK/include/JetTable.h
../nxdeps/include/STK/include/LentPitShift.h
../nxdeps/include/STK/include/Mandolin.h
../nxdeps/include/STK/include/Mesh2D.h
../nxdeps/include/STK/include/Messager.h
../nxdeps/include/STK/include/MidiFileIn.h
../nxdeps/include/STK/include/Modal.h
../nxdeps/include/STK/include/ModalBar.h
../nxdeps/include/STK/include/Modulate.h
../nxdeps/include/STK/include/Moog.h
../nxdeps/include/STK/include/Mutex.h
../nxdeps/include/STK/include/Noise.h
../nxdeps/include/STK/include/NRev.h
../nxdeps/include/STK/include/OnePole.h
../nxdeps/include/STK/include/OneZero.h
../nxdeps/include/STK/include/PercFlut.h
../nxdeps/include/STK/include/Phonemes.h
../nxdeps/include/STK/include/PitShift.h
../nxdeps/include/STK/include/Plucked.h
../nxdeps/include/STK/include/PoleZero.h
../nxdeps/include/STK/include/PRCRev.h
../nxdeps/include/STK/include/ReedTable.h
../nxdeps/include/STK/include/Resonate.h
../nxdeps/include/STK/include/Rhodey.h
../nxdeps/include/STK/include/RtAudio.h
../nxdeps/include/STK/include/RtError.h
../nxdeps/include/STK/include/RtMidi.h
../nxdeps/include/STK/include/RtWvIn.h
../nxdeps/include/STK/include/RtWvOut.h
../nxdeps/include/STK/include/Sampler.h
../nxdeps/include/STK/include/Saxofony.h
../nxdeps/include/STK/include/Shakers.h
../nxdeps/include/STK/include/Simple.h
../nxdeps/include/STK/include/SineWave.h
../nxdeps/include/STK/include/SingWave.h
../nxdeps/include/STK/include/Sitar.h
../nxdeps/include/STK/include/Skini.h
../nxdeps/include/STK/include/SKINI.msg
../nxdeps/include/STK/include/SKINI.tbl
../nxdeps/include/STK/include/Socket.h
../nxdeps/include/STK/include/Sphere.h
../nxdeps/include/STK/include/StifKarp.h
../nxdeps/include/STK/include/Stk.h
../nxdeps/include/STK/include/TapDelay.h
../nxdeps/include/STK/include/TcpClient.h			
../nxdeps/include/STK/include/TcpServer.h			
../nxdeps/include/STK/include/Thread.h			
../nxdeps/include/STK/include/TubeBell.h			
../nxdeps/include/STK/include/Twang.h
../nxdeps/include/STK/include/TwoPole.h			
../nxdeps/include/STK/include/TwoZero.h
../nxdeps/include/STK/include/UdpSocket.h		
../nxdeps/include/STK/include/Vector3D.h		
../nxdeps/include/STK/include/Voicer.h		
../nxdeps/include/STK/include/VoicForm.h		
../nxdeps/include/STK/include/Whistle.h			
../nxdeps/include/STK/include/Wurley.h			
../nxdeps/include/STK/include/WvIn.h			
../nxdeps/include/STK/include/WvOut.h
./NxSound.h		
./NxSound_Device.h		
./NxSound_Device_Input.h	
./NxSound_Device_Output.h
./NxSound_Singleton.h	
./NxSound_Headers.h			
./NxSound_Log.h		
./NxSound_Media_DataTypes.h		
./NxSound_Plateform.h		
./NxSound_Prerequisites.h		
./NxSound_Stable_Headers.h		
./NxSound_Utilities.h		
./NxSoundPreCompiled.h
./NxSound_Effect_Base.h
./NxSound_Effect_VSTHost.h
./NxSound_Effect_VSTPlugin.h
./NxSound_Effect_VSTPluginPreset.h
./NxSound_Effect_3D.h			
./NxSound_Effect_BeatDetector.h			
./NxSound_Effect_Chorus.h			
./NxSound_Effect_Echo.h
./NxSound_Effect_Granulate.h			
./NxSound_Effect_OggBroadCaster.h
./NxSound_Effect_Shift.h
./NxSound_FFT.h
./FileRead.h
./FileReadFlac.h
./FileReadMp3.h
./FileReadOgg.h
./FileReadWav.h
./NxSound_Media_File.h
./NxSound_Media_Inst_MODAL.h
./NxSound_Media_File_FLAC.h			
./NxSound_Media_File_MP3.h
./NxSound_Media_File_OGG.h			
./NxSound_Media_File_VSTi.h
./NxSound_Media_File_WAV.h
./NxSound_Media_File_Buffer.h 
)

SOURCE_GROUP("LIBRAIRIES\\SPEEX\\Sources" FILES 
../nxdeps/include/speex/resample.c
)

SOURCE_GROUP("LIBRAIRIES\\STK\\Sources" FILES 
../nxdeps/include/STK/src/ADSR.cpp
../nxdeps/include/STK/src/Asymp.cpp
../nxdeps/include/STK/src/BandedWG.cpp			
../nxdeps/include/STK/src/BeeThree.cpp			
../nxdeps/include/STK/src/BiQuad.cpp			
../nxdeps/include/STK/src/Blit.cpp			
../nxdeps/include/STK/src/BlitSaw.cpp
../nxdeps/include/STK/src/BlitSquare.cpp
../nxdeps/include/STK/src/BlowBotl.cpp
../nxdeps/include/STK/src/BlowHole.cpp			
../nxdeps/include/STK/src/Bowed.cpp			
../nxdeps/include/STK/src/Brass.cpp
../nxdeps/include/STK/src/Chorus.cpp			
../nxdeps/include/STK/src/Clarinet.cpp			
../nxdeps/include/STK/src/Delay.cpp
../nxdeps/include/STK/src/DelayA.cpp			
../nxdeps/include/STK/src/DelayL.cpp		
../nxdeps/include/STK/src/Drummer.cpp			
../nxdeps/include/STK/src/Echo.cpp
../nxdeps/include/STK/src/Envelope.cpp
../nxdeps/include/STK/src/FileLoop.cpp			
../nxdeps/include/STK/src/FileRead.cpp			
../nxdeps/include/STK/src/FileWrite.cpp
../nxdeps/include/STK/src/FileWvIn.cpp			
../nxdeps/include/STK/src/FileWvOut.cpp
../nxdeps/include/STK/src/Fir.cpp
../nxdeps/include/STK/src/Flute.cpp
../nxdeps/include/STK/src/FM.cpp
../nxdeps/include/STK/src/FMVoices.cpp			
../nxdeps/include/STK/src/FormSwep.cpp
../nxdeps/include/STK/src/Granulate.cpp
../nxdeps/include/STK/src/HevyMetl.cpp
../nxdeps/include/STK/src/Iir.cpp
../nxdeps/include/STK/src/InetWvIn.cpp		
../nxdeps/include/STK/src/InetWvOut.cpp			
../nxdeps/include/STK/src/JCRev.cpp
../nxdeps/include/STK/src/LentPitShift.cpp
../nxdeps/include/STK/src/Mandolin.cpp		
../nxdeps/include/STK/src/Mesh2D.cpp			
../nxdeps/include/STK/src/Messager.cpp			
../nxdeps/include/STK/src/MidiFileIn.cpp
../nxdeps/include/STK/src/Modal.cpp
../nxdeps/include/STK/src/ModalBar.cpp			
../nxdeps/include/STK/src/Modulate.cpp	
../nxdeps/include/STK/src/Moog.cpp	
../nxdeps/include/STK/src/Mutex.cpp			
../nxdeps/include/STK/src/Noise.cpp		
../nxdeps/include/STK/src/NRev.cpp			
../nxdeps/include/STK/src/OnePole.cpp			
../nxdeps/include/STK/src/OneZero.cpp			
../nxdeps/include/STK/src/PercFlut.cpp		
../nxdeps/include/STK/src/Phonemes.cpp			
../nxdeps/include/STK/src/PitShift.cpp			
../nxdeps/include/STK/src/Plucked.cpp			
../nxdeps/include/STK/src/PoleZero.cpp			
../nxdeps/include/STK/src/PRCRev.cpp			
../nxdeps/include/STK/src/Resonate.cpp
../nxdeps/include/STK/src/Rhodey.cpp		
../nxdeps/include/STK/src/RtAudio.cpp
../nxdeps/include/STK/src/RtMidi.cpp			
../nxdeps/include/STK/src/RtWvIn.cpp			
../nxdeps/include/STK/src/RtWvOut.cpp			
../nxdeps/include/STK/src/Sampler.cpp			
../nxdeps/include/STK/src/Saxofony.cpp			
../nxdeps/include/STK/src/Shakers.cpp
../nxdeps/include/STK/src/Simple.cpp			
../nxdeps/include/STK/src/SineWave.cpp			
../nxdeps/include/STK/src/SingWave.cpp			
../nxdeps/include/STK/src/Sitar.cpp			
../nxdeps/include/STK/src/Skini.cpp		
../nxdeps/include/STK/src/Socket.cpp			
../nxdeps/include/STK/src/Sphere.cpp			
../nxdeps/include/STK/src/StifKarp.cpp			
../nxdeps/include/STK/src/Stk.cpp			
../nxdeps/include/STK/src/TapDelay.cpp			
../nxdeps/include/STK/src/TcpClient.cpp
../nxdeps/include/STK/src/TcpServer.cpp
../nxdeps/include/STK/src/Thread.cpp
../nxdeps/include/STK/src/TubeBell.cpp
../nxdeps/include/STK/src/Twang.cpp
../nxdeps/include/STK/src/TwoPole.cpp
../nxdeps/include/STK/src/TwoZero.cpp
../nxdeps/include/STK/src/UdpSocket.cpp	
../nxdeps/include/STK/src/Voicer.cpp
../nxdeps/include/STK/src/VoicForm.cpp
../nxdeps/include/STK/src/Whistle.cpp
../nxdeps/include/STK/src/Wurley.cpp
)

SOURCE_GROUP("LIBRAIRIES\\STK\\Headers" FILES 
../nxdeps/include/STK/include/ADSR.h
../nxdeps/include/STK/include/Asymp.h
../nxdeps/include/STK/include/BandedWG.h
../nxdeps/include/STK/include/BeeThree.h
../nxdeps/include/STK/include/BiQuad.h
../nxdeps/include/STK/include/Blit.h
../nxdeps/include/STK/include/BlitSaw.h
../nxdeps/include/STK/include/BlitSquare.h
../nxdeps/include/STK/include/BlowBotl.h
../nxdeps/include/STK/include/BlowHole.h
../nxdeps/include/STK/include/Bowed.h
../nxdeps/include/STK/include/BowTable.h
../nxdeps/include/STK/include/Brass.h
../nxdeps/include/STK/include/Chorus.h
../nxdeps/include/STK/include/Clarinet.h
../nxdeps/include/STK/include/Cubic.h
../nxdeps/include/STK/include/Delay.h
../nxdeps/include/STK/include/DelayA.h
../nxdeps/include/STK/include/DelayL.h
../nxdeps/include/STK/include/Drummer.h
../nxdeps/include/STK/include/Echo.h
../nxdeps/include/STK/include/Effect.h
../nxdeps/include/STK/include/Envelope.h
../nxdeps/include/STK/include/FileLoop.h
../nxdeps/include/STK/include/FileRead.h
../nxdeps/include/STK/include/FileWrite.h
../nxdeps/include/STK/include/FileWvIn.h
../nxdeps/include/STK/include/FileWvOut.h
../nxdeps/include/STK/include/Filter.h
../nxdeps/include/STK/include/Fir.h
../nxdeps/include/STK/include/Flute.h
../nxdeps/include/STK/include/FM.h
../nxdeps/include/STK/include/FMVoices.h
../nxdeps/include/STK/include/FormSwep.h
../nxdeps/include/STK/include/Function.h
../nxdeps/include/STK/include/Generator.h
../nxdeps/include/STK/include/Granulate.h
../nxdeps/include/STK/include/HevyMetl.h
../nxdeps/include/STK/include/Iir.h
../nxdeps/include/STK/include/InetWvIn.h
../nxdeps/include/STK/include/InetWvOut.h
../nxdeps/include/STK/include/Instrmnt.h
../nxdeps/include/STK/include/JCRev.h
../nxdeps/include/STK/include/JetTable.h
../nxdeps/include/STK/include/LentPitShift.h
../nxdeps/include/STK/include/Mandolin.h
../nxdeps/include/STK/include/Mesh2D.h
../nxdeps/include/STK/include/Messager.h
../nxdeps/include/STK/include/MidiFileIn.h
../nxdeps/include/STK/include/Modal.h
../nxdeps/include/STK/include/ModalBar.h
../nxdeps/include/STK/include/Modulate.h
../nxdeps/include/STK/include/Moog.h
../nxdeps/include/STK/include/Mutex.h
../nxdeps/include/STK/include/Noise.h
../nxdeps/include/STK/include/NRev.h
../nxdeps/include/STK/include/OnePole.h
../nxdeps/include/STK/include/OneZero.h
../nxdeps/include/STK/include/PercFlut.h
../nxdeps/include/STK/include/Phonemes.h
../nxdeps/include/STK/include/PitShift.h
../nxdeps/include/STK/include/Plucked.h
../nxdeps/include/STK/include/PoleZero.h
../nxdeps/include/STK/include/PRCRev.h
../nxdeps/include/STK/include/ReedTable.h
../nxdeps/include/STK/include/Resonate.h
../nxdeps/include/STK/include/Rhodey.h
../nxdeps/include/STK/include/RtAudio.h
../nxdeps/include/STK/include/RtError.h
../nxdeps/include/STK/include/RtMidi.h
../nxdeps/include/STK/include/RtWvIn.h
../nxdeps/include/STK/include/RtWvOut.h
../nxdeps/include/STK/include/Sampler.h
../nxdeps/include/STK/include/Saxofony.h
../nxdeps/include/STK/include/Shakers.h
../nxdeps/include/STK/include/Simple.h
../nxdeps/include/STK/include/SineWave.h
../nxdeps/include/STK/include/SingWave.h
../nxdeps/include/STK/include/Sitar.h
../nxdeps/include/STK/include/Skini.h
../nxdeps/include/STK/include/SKINI.msg
../nxdeps/include/STK/include/SKINI.tbl
../nxdeps/include/STK/include/Socket.h
../nxdeps/include/STK/include/Sphere.h
../nxdeps/include/STK/include/StifKarp.h
../nxdeps/include/STK/include/Stk.h
../nxdeps/include/STK/include/TapDelay.h
../nxdeps/include/STK/include/TcpClient.h			
../nxdeps/include/STK/include/TcpServer.h			
../nxdeps/include/STK/include/Thread.h			
../nxdeps/include/STK/include/TubeBell.h			
../nxdeps/include/STK/include/Twang.h
../nxdeps/include/STK/include/TwoPole.h			
../nxdeps/include/STK/include/TwoZero.h
../nxdeps/include/STK/include/UdpSocket.h		
../nxdeps/include/STK/include/Vector3D.h		
../nxdeps/include/STK/include/Voicer.h		
../nxdeps/include/STK/include/VoicForm.h		
../nxdeps/include/STK/include/Whistle.h			
../nxdeps/include/STK/include/Wurley.h			
../nxdeps/include/STK/include/WvIn.h			
../nxdeps/include/STK/include/WvOut.h
)

SOURCE_GROUP("Source Files\\Effects" FILES 
NxSound_Effect_Base.cpp
)

SOURCE_GROUP("Source Files\\Effects\\EffectsDsp" FILES 
./NxSound_Effect_3D.cpp
./NxSound_Effect_BeatDetector.cpp
./NxSound_Effect_Chorus.cpp
./NxSound_Effect_Echo.cpp
./NxSound_Effect_Granulate.cpp
./NxSound_Effect_OggBroadCaster.cpp
./NxSound_Effect_Shift.cpp
)

SOURCE_GROUP("Source Files\\Effects\\EffectsVST" FILES 
./NxSound_Effect_VSTHost.cpp
./NxSound_Effect_VSTPluginPreset.cpp
)

SOURCE_GROUP("Source Files\\Effects\\FFT" FILES 
./NxSound_FFT.cpp
)

SOURCE_GROUP("Source Files\\FileRead" FILES 
./FileReadBase.cpp
./FileReadFlac.cpp
./FileReadMp3.cpp
./FileReadOgg.cpp
./FileReadWav.cpp
)

SOURCE_GROUP("Source Files\\FileTypes" FILES 
NxSound_Media_File.cpp
)

SOURCE_GROUP("Source Files\\FileTypes\\Instruments" FILES 
NxSound_Media_Inst_MODAL.cpp
)

SOURCE_GROUP("Source Files\\FileTypes\\Streams" FILES 
./NxSound_Media_File_FLAC.cpp
./NxSound_Media_File_MP3.cpp
./NxSound_Media_File_OGG.cpp
./NxSound_Media_File_VSTi.cpp
./NxSound_Media_File_WAV.cpp
./NxSound_Media_File_Buffer.cpp
)

SOURCE_GROUP("Header Files\\Effects" FILES 
NxSound_Effect_Base.h
)

SOURCE_GROUP("Header Files\\Effects\\EffectsDsp" FILES 
./NxSound_Effect_3D.h			
./NxSound_Effect_BeatDetector.h			
./NxSound_Effect_Chorus.h			
./NxSound_Effect_Echo.h
./NxSound_Effect_Granulate.h			
./NxSound_Effect_OggBroadCaster.h
./NxSound_Effect_Shift.h
)

SOURCE_GROUP("Header Files\\Effects\\EffectsVST" FILES 
./NxSound_Effect_VSTHost.h
./NxSound_Effect_VSTPlugin.h
./NxSound_Effect_VSTPluginPreset.h
)

SOURCE_GROUP("Header Files\\Effects\\FFT" FILES 
./NxSound_FFT.h
)

SOURCE_GROUP("Header Files\\FileTypes" FILES 
./NxSound_Media_File.h
)

SOURCE_GROUP("Header Files\\FileTypes\\Instruments" FILES 
./NxSound_Media_Inst_MODAL.h
)

SOURCE_GROUP("Header Files\\FileTypes\\Streams" FILES 
./NxSound_Media_File_FLAC.h			
./NxSound_Media_File_MP3.h
./NxSound_Media_File_OGG.h			
./NxSound_Media_File_VSTi.h
./NxSound_Media_File_WAV.h
./NxSound_Media_File_Buffer.h
)



# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../Lib )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../Lib )

# warning level
IF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
   # Use the lowest warning level for visual studio.
   SET(CMAKE_CXX_WARNING_LEVEL 0)
   IF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     STRING(REGEX REPLACE "/W[0-4]" "/W0"
            CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
   ELSE(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
   ENDIF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
ENDIF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")



#SOURCE_GROUP(foo_headers REGULAR_EXPRESSION "foo.*//.h")
#ADD_EXECUTABLE(NxSound ${NxSound_src})

include_directories(../nxdeps/include) 
include_directories(../nxdeps/include/VST)
include_directories(../nxdeps/include/STK/include)
include_directories(../nxdeps/include/STK/src/include)
include_directories(../nxdeps/include/speex)

set(NxSound_DEF "NXSOUND_NONCLIENT_BUILD;" )

set(NxSound_LINK_FLAGS "" )

#speex
ADD_DEFINITIONS(-DFLOATING_POINT)
ADD_DEFINITIONS(-DOUTSIDE_SPEEX)

if( MSVC )

message(  "System is MSVC !" )
	SET( NxSound_src ${NxSound_src} ./NxSound_Effect_VSTWindow.cpp )
	SET( NxSound_src ${NxSound_src} ./NxSound_Effect_VSTPlugin.cpp )

	SOURCE_GROUP("Source Files\\Effects\\EffectsVST" FILES ./NxSound_Effect_VSTWindow.cpp )
	SOURCE_GROUP("Source Files\\Effects\\EffectsVST" FILES ./NxSound_Effect_VSTPlugin.cpp )


	# from http://www.cmake.org/pipermail/cmake/2007-November/017372.html
	SET( NxSound_LINK_FLAGS "/NODEFAULTLIB:libcmt.lib")
	SET( NxSound_DEF "${NxSound_DEF}WIN32;_WINDOWS;_USRDLL;WIN32_LEAN_AND_MEAN;__WINDOWS_DS__;__WINDOWS_MM__;")
	SET( NxSound_Deps "../NxDeps/lib/release/" )

 
	SET(NxSound_EXTRALIBS 
	../NxDeps/lib/dsound
	../NxDeps/lib/openal32
	${NxSound_Deps}kissfft
	${NxSound_Deps}ogg
	${NxSound_Deps}vorbis
	${NxSound_Deps}vorbisfile
	${NxSound_Deps}libmad
	${NxSound_Deps}libflac
	winmm
	wsock32 )


	
elseif( CYGWIN )	

	#__LINUX_ALSA__, __UNIX_JACK__, and/or __LINUX_OSS__



	SET( NxSound_DEF "${NxSound_DEF}__LINUX_OSS__;_USRDLL;") 
ADD_DEFINITIONS(-D_WIN32)
ADD_DEFINITIONS(-DCYGWIN)
ADD_DEFINITIONS(-D__CYGWIN__)
include_directories(../nxdeps/include/libFlac)

elseif( UNIX )	

	message(  "System is Linux !" )
	 
 	#find_package(Threads REQUIRED)
	#list(APPEND NxSound_EXTRALIBS ${CMAKE_THREAD_LIBS_INIT})
 

	ADD_DEFINITIONS(-D__OPENSLES__) #for stk
	ADD_DEFINITIONS(-D__OS_LINUX__) # for stk thread.h
	ADD_DEFINITIONS(-D__LITTLE_ENDIAN__) #for stk
 


	ADD_DEFINITIONS(-D__TMS320C6X__) #for ogg
	include_directories(../nxdeps/include/libFlac)
 
	SET( NxSound_src ${NxSound_src} ./NxSound_Effect_VSTWindowLinux.cpp )
	SET( NxSound_src ${NxSound_src} ./NxSound_Effect_VSTPlugin.cpp )

	SOURCE_GROUP("Source Files\\Effects\\EffectsVST" FILES ./NxSound_Effect_VSTWindowLinux.cpp )
	SOURCE_GROUP("Source Files\\Effects\\EffectsVST" FILES ./NxSound_Effect_VSTPlugin.cpp )



else()

message(  "System is Not defined !" )

	SET( NxSound_src ${NxSound_src} ./NxSound_Effect_VSTWindow.mm )
	SET( NxSound_src ${NxSound_src} ./NxSound_Effect_VSTPlugin.cpp )

	SOURCE_GROUP( "Source Files\\Effects\\EffectsVST" FILES ./NxSound_Effect_VSTWindow.mm )
	SOURCE_GROUP("Source Files\\Effects\\EffectsVST" FILES ./NxSound_Effect_VSTPlugin.cpp )
	SET(NxSound_DEF "${NxSound_DEF}__MACOSX_CORE__;")
	SET(NxSound_EXTRALIBS /System/Library/Frameworks/CoreAudio.framework )
    SET(NxSound_EXTRALIBS /System/Library/Frameworks/AudioUnit.framework ${NxSound_EXTRALIBS})
	SET(NxSound_EXTRALIBS /System/Library/Frameworks/CoreMidi.framework ${NxSound_EXTRALIBS})
    SET(NxSound_EXTRALIBS /System/Library/Frameworks/ApplicationServices.framework ${NxSound_EXTRALIBS})

	# Some versions of OSX may need the AudioToolbox framework. Add it if it's found.
    FIND_LIBRARY(AUDIOTOOLBOX_LIBRARY NAMES AudioToolbox PATHS ~/Library/Frameworks /Library/Frameworks /System/Library/Frameworks )
	IF(AUDIOTOOLBOX_LIBRARY)
    	SET(NxSound_EXTRALIBS ${AUDIOTOOLBOX_LIBRARY} ${NxSound_EXTRALIBS})
    ENDIF()
	include_directories(../nxdeps/include/libFlac)
endif()

 
set(NxSound_DEF "${NxSound_DEF}FLAC_API_EXPORTS;FLAC__HAS_OGG;FLAC__CPU_IA32;FLAC__HAS_NASM;FLAC__USE_3DNOW;VERSION=\"1.2.0\";")
set(NxSound_DEF "${NxSound_DEF}AL_LIBTYPE_STATIC;FPM_INTEL;_EXPORTING;__LITTLE_ENDIAN__;_USE_MATH_DEFINES;")
set(NxSound_DEF "${NxSound_DEF}LIBVORBIS_EXPORTS;LIBOGG_EXPORTS;FFTW_SINGLE;HAVE_SSE;")

if( MSVC )
add_library(${NxSound_LIBRARY} SHARED ${NxSound_src})
else()
add_library(${NxSound_LIBRARY} STATIC ${NxSound_src})
endif()


set_target_properties(${NxSound_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${NxSound_DEF}" LINK_FLAGS_RELEASE "${NxSound_LINK_FLAGS}")

set_target_properties(${NxSound_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${NxSound_DEF}" LINK_FLAGS_DEBUG "${NxSound_LINK_FLAGS}")

target_link_libraries(${NxSound_LIBRARY} ${NxSound_EXTRALIBS})



