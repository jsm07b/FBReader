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

#ifndef __CBRSTREAM_H__
#define __CBRSTREAM_H__

/*
 * CbrStream.h
 *
 * This class extracts images from a RAR archive and stores them in memory.
 *
 * A vector of CbrPage's from the RAR will be stored in memory. Always call
 * closeArchive() even when an error occurs opening or extracting. 
 */

#include <vector>
#include "CbrPage.h"

#include "../unrar/dll.hpp"

class CbrStream {

public:
	CbrStream();
	~CbrStream();

	//Begins the process of opening a RAR archive
	//	Establishes callback function
	//	Sets class to store extracted data (CbrPage)
	bool openArchive(char *name);

	//Closes RAR archive and releases allocated memory.
	bool closeArchive(void);

	//Will populate allPages vector with CbrPages extracted
	//from the RAR archive.
	bool extractArchive(void);

	//Returns total number of CbrPages extracted from the RAR archive
	int numPages(void) const;

	//Returns a specific CbrPage
	const CbrPage* getPage(unsigned int index) const;
	
private:
	//Struct used by RAROpenArchiveEx() from UnRAR
	RAROpenArchiveDataEx OpenArchiveData;
	
	//Stores a handle to the archive data
	HANDLE hArcData;

	//Struct used by RARReadHeaderEx() from UnRAR
	RARHeaderDataEx HeaderData;

	//Temporary page that extracted data is put into
	CbrPage page;

	//Vector of every page in the RAR archive
	std::vector<CbrPage *> allPages;
};

#endif /* __CBRSTREAM_H__ */
