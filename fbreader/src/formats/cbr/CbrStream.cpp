/*
 * Copyright (C) 2004-2010 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <cstddef>
#include <cstring>

#include "CbrStream.h"
#include "CbrBookReader.h"

//This is a callback method that UnRAR requires to extract.  
//The max amount of data sent each call will not exceed the
//maximum dictionary size (4 MB in RAR 3.0). 
//	msg - Event type. Multivolume and passwords not supported.
//	userData - Class instance that is shared between all calls
//	rarBuffer - Address pointing to unpacked data.
//	bytesProcessed - Size of unpacked data.
static int CALLBACK unrarCallback(UINT msg, LPARAM userData, LPARAM rarBuffer, LPARAM bytesProcessed) {
	if (msg != UCM_PROCESSDATA) {
		return -1;
	}

	CbrPage *tmpPage = (CbrPage*)userData;
	tmpPage->appendToBuffer((char*)rarBuffer, bytesProcessed);
	return 1;
}

CbrStream::CbrStream() {
}

CbrStream::~CbrStream() {
	//Free the memory of the CbrPage* in the vector
	for (int i = 0; i < allPages.size(); ++i) {
    		delete allPages[i];
  	}
  	allPages.clear();
}

bool CbrStream::openArchive(char *name) {
	//Initialize the archive's struct
	OpenArchiveData.ArcName = name;
	OpenArchiveData.ArcNameW = NULL;
	OpenArchiveData.OpenMode = RAR_OM_EXTRACT;
	OpenArchiveData.CmtBuf = NULL;
	OpenArchiveData.Callback = &unrarCallback;
	OpenArchiveData.UserData = (LPARAM)&page;
	memset(&OpenArchiveData.Reserved, 0, sizeof(OpenArchiveData.Reserved));

	//Initialize the header data
	HeaderData.CmtBuf = NULL;
	HeaderData.CmtBufSize = 0;
	memset(&HeaderData.Reserved, 0, sizeof(HeaderData.Reserved));

	//Attempt to open archive
	hArcData = RAROpenArchiveEx(&OpenArchiveData);
	if (!hArcData || OpenArchiveData.OpenResult != 0) {
		return false;
	}
	return true;
}

bool CbrStream::closeArchive(void) {
	if (RARCloseArchive(hArcData) != 0) {
		return false;
	}
	return true;
}

bool CbrStream::extractArchive(void) {
	//Loop until there are no more archives
	while (RARReadHeaderEx(hArcData, &HeaderData) == 0) {

		//Initialize the CbrPage with data from the new file
    		page.init(HeaderData);

		//Extract
    		if (RARProcessFile(hArcData, RAR_TEST, NULL, NULL) != 0) {
			return false;
		}

		//Add the page to the collection
    		allPages.push_back(new CbrPage(page));
	}
	return true;
}

int CbrStream::numPages(void) const {
	return allPages.size();
}

const CbrPage* CbrStream::getPage(unsigned int index) const {
	if (index < allPages.size()) {
		return allPages[index];
	}
}

