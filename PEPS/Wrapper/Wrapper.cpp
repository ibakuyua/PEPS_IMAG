// Il s'agit du fichier DLL principal.

#include "stdafx.h"

#include "Wrapper.h"
#include <iostream>
#include <string>


namespace Wrapper {
	Price::Price(){
		this->prix = 10;
	}

	Price::~Price(){

	}

	void Price::test(){
		this->prix *= 2;
	}

}