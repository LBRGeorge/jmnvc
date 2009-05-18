# Microsoft Developer Studio Project File - Name="rcon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=rcon - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rcon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rcon.mak" CFG="rcon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rcon - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "rcon - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cwcl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rcon - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=cwlink.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"c:\gtavc\rcon.exe"

!ELSEIF  "$(CFG)" == "rcon - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=cwlink.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "rcon - Win32 Release"
# Name "rcon - Win32 Debug"
# Begin Group "raknet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\raknet\AES128.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\AsynchronousFileIO.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\AsynchronousFileIO.h
# End Source File
# Begin Source File

SOURCE=..\raknet\BitStream.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\CheckSum.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\CheckSum.h
# End Source File
# Begin Source File

SOURCE=..\raknet\DataBlockEncryptor.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObject.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObject.h
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObjectHeader.h
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObjectManager.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObjectManager.h
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObjectStub.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObjectStub.h
# End Source File
# Begin Source File

SOURCE=..\raknet\EncodeClassName.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\EncodeClassName.h
# End Source File
# Begin Source File

SOURCE=..\raknet\ExtendedOverlappedPool.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\ExtendedOverlappedPool.h
# End Source File
# Begin Source File

SOURCE=..\raknet\GetTime.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\GetTime.h
# End Source File
# Begin Source File

SOURCE=..\raknet\HuffmanEncodingTree.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\HuffmanEncodingTree.h
# End Source File
# Begin Source File

SOURCE=..\raknet\HuffmanEncodingTreeFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\HuffmanEncodingTreeFactory.h
# End Source File
# Begin Source File

SOURCE=..\raknet\HuffmanEncodingTreeNode.h
# End Source File
# Begin Source File

SOURCE=..\raknet\InternalPacketPool.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\Multiplayer.h
# End Source File
# Begin Source File

SOURCE=..\raknet\NetworkObject.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\NetworkObject.h
# End Source File
# Begin Source File

SOURCE=..\raknet\NetworkTypes.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\PacketPool.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\PacketPool.h
# End Source File
# Begin Source File

SOURCE=..\raknet\Queue.h
# End Source File
# Begin Source File

SOURCE=..\raknet\RakClient.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RakNetStatistics.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RakNetworkFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RakPeer.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RakServer.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RakServer.h
# End Source File
# Begin Source File

SOURCE=..\raknet\RakServerInterface.h
# End Source File
# Begin Source File

SOURCE=..\raknet\Rand.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\ReliabilityLayer.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RPCNode.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\SHA1.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\SimpleMutex.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\SocketLayer.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\StringCompressor.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\StringCompressor.h
# End Source File
# Begin Source File

SOURCE=..\raknet\Types.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\rconclient.cpp
# End Source File
# Begin Source File

SOURCE=.\rconclient.h
# End Source File
# End Target
# End Project
