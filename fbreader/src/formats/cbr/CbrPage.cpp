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

#include "CbrPage.h"

#include "../unrar/dll.hpp"


CbrPage::CbrPage() : curSize(0), buffer(NULL) {
}

CbrPage::CbrPage(const CbrPage &page) : name(page.name), totalSize(page.totalSize), curSize(page.curSize), buffer(NULL) {
	freeBuffer();
	buffer = new char[page.curSize];
	memcpy(buffer, page.buffer, page.curSize); 
}

CbrPage::~CbrPage() {
	freeBuffer();
}

CbrPage& CbrPage::operator=(const CbrPage &page) {
	if (this != &page) {
		name = page.name;
		totalSize = page.totalSize;
		curSize = page.curSize;
		freeBuffer();
		buffer = new char[page.curSize];
		memcpy(buffer, page.buffer, page.curSize);
	}
	return *this;
}

void CbrPage::init(RARHeaderDataEx HeaderData) {
	name = HeaderData.FileName;
	totalSize = HeaderData.UnpSize;
	curSize = 0;
	freeBuffer();
	buffer = new char[HeaderData.UnpSize];
}

void CbrPage::freeBuffer(void) {
	if (buffer != NULL) {
		delete[] buffer;
		buffer = NULL;
	}
}

void CbrPage::setName(std::string str) {
	name = str;
}

const std::string CbrPage::getName(void) const {
	return name;
}

void CbrPage::setCurSize(unsigned int size) {
	curSize = size;
}

unsigned int CbrPage::getCurSize(void) const {
	return curSize;
}

void CbrPage::setTotalSize(unsigned int size) {
	totalSize = size;
}
unsigned int CbrPage::getTotalSize(void) const {
	return totalSize;
}

bool CbrPage::appendToBuffer(char *buf, unsigned int size) {

	//If new size is larger than what UnRAR said, something got corrupted
	if (curSize + size > totalSize) {
		return false;
	}

	//The buffer fills up with more and more data each call.
	//Get the location where the last call left off, and continue filling.
	memcpy(buffer+curSize, buf, size);
	curSize += size;
	return true;
}

const char* CbrPage::getBuffer(void) const {
	return buffer;
}
