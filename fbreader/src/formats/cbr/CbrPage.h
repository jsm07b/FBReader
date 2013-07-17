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

#ifndef __CBRPAGE_H__
#define __CBRPAGE_H__

/*
 * CbrPage.h
 *
 * This class is used to store an image file (bmp, gif, jpg, etc.)
 *
 * This class currently supports RAR archives using the unrar source
 * code from RARLAB. It provides a simple container for an extracted
 * image.
 *
 * The image data is stored in a char *buffer
 */


#include <string>

class RARHeaderDataEx;

class CbrPage {

public:

	CbrPage();
	CbrPage(const CbrPage &page);
	~CbrPage();
	CbrPage& operator=(const CbrPage &page);

	// Parameters:
	// 	HeaderData - Struct defined in unrar library
	// Description:
	// 	Sets our 'name' and 'totalSize' with
	// 	RAR archive's name and size. Also allocates enough
	// 	memory to 'buffer' to store image
	void init(RARHeaderDataEx HeaderData);

	// Deletes the dynamic memory in buffer
	void freeBuffer(void);

	// Mutator/Accessor for name
	void setName(std::string str);
	const std::string getName(void) const;

	// Mutator/Accessor for curSize
	void setCurSize(unsigned int size);
	unsigned int getCurSize(void) const;

	// Mutator/Accessor for totalSize
	void setTotalSize(unsigned int size);
	unsigned int getTotalSize(void) const;

	// Parameters:
	// 	 buf - Buffer of data to add to internal buffer
	//	size - Number of bytes buf contains
	// Description:
	// 	This method should be used in UnRAR's extraction callback.
	// 	Each call will append data to 'buffer' sequentially until
	// 	'buffer' is filled to max size.
	bool appendToBuffer(char *buf, unsigned int size);

	// Returns the buffer pointer as const
	const char* getBuffer(void) const;

private:
	//Name of file
	std::string name;

	//Total size of file as reported by UnRAR
	unsigned int totalSize;

	//Current size of the file [0-totalSize]
	unsigned int curSize;

	//Data buffer used to store image
	char *buffer;
};

#endif /* __CBRPAGE_H__ */
