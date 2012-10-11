/*
 *  PlistBRRFile.cpp
 *  Chip700
 *
 *  Created by ���c ���F on 12/10/11.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Chip700defines.h"
#include "PlistBRRFile.h"

#if MAC
//-----------------------------------------------------------------------------
PlistBRRFile::PlistBRRFile( const char *path, bool isWriteable )
: FileAccess(path, isWriteable)
{
}

//-----------------------------------------------------------------------------
PlistBRRFile::~PlistBRRFile()
{
	if ( mIsLoaded ) {
		CFRelease(mPropertydata);
	}
}

//-----------------------------------------------------------------------------
bool PlistBRRFile::Load()
{
	if ( IsLoaded() ) {
		CFRelease(mPropertydata);
	}
	if ( mPath == NULL ) return false;
	//CFURL���쐬
	CFURLRef	path = CFURLCreateFromFileSystemRepresentation(NULL, (UInt8*)mPath, strlen(mPath), false);
	
	//�ۑ����ꂽ�p�b�`(.brr�t�@�C��)�̓ǂݍ���
	CFReadStreamRef	filestream = CFReadStreamCreateWithFile(NULL, path);
	if (CFReadStreamOpen(filestream)) {
		CFPropertyListFormat	format = kCFPropertyListBinaryFormat_v1_0;
		mPropertydata = CFPropertyListCreateFromStream(NULL,filestream,0,
													   kCFPropertyListImmutable,
													   &format,NULL);
		if ( mPropertydata ) {
			mIsLoaded = true;
		}
		else {
			mIsLoaded = false;
		}
		CFReadStreamClose(filestream);
	}
	CFRelease(filestream);
	
	return mIsLoaded;
}

//-----------------------------------------------------------------------------
bool PlistBRRFile::Write()
{
	if ( IsLoaded() == false ) return false;
	if ( mPath == NULL ) return false;
	
	//CFURL���쐬
	CFURLRef	savefile = CFURLCreateFromFileSystemRepresentation(NULL, (UInt8*)mPath, strlen(mPath), false);

	//�o�C�i���`���ɕϊ�
	CFWriteStreamRef	filestream = CFWriteStreamCreateWithFile(NULL,savefile);
	if (CFWriteStreamOpen(filestream)) {
		CFPropertyListWriteToStream(mPropertydata,filestream,kCFPropertyListBinaryFormat_v1_0,NULL);
		CFWriteStreamClose(filestream);
	}
	CFRelease(filestream);
	CFRelease(savefile);
	
	return true;
}

//-----------------------------------------------------------------------------
void PlistBRRFile::SetPlistData( CFPropertyListRef propertydata )
{
	if ( mIsLoaded ) {
		CFRelease(mPropertydata);
	}
	mPropertydata = propertydata;
	CFRetain(mPropertydata);
	mIsLoaded = true;
}

//-----------------------------------------------------------------------------
CFPropertyListRef PlistBRRFile::GetPlistData() const
{
	if ( IsLoaded() == false ) return NULL;
	
	return mPropertydata;
}
#endif