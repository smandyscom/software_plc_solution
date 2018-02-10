#include<LDE_ELEMENT.h>


ldElement::ldElement(const ldElement& source)
{
	//-------------------------------------------
	//	copy-constructor , for multi-thread usage
	//-------------------------------------------
	this->type = source.type;
	this->v = NULL;			
	if (source.type == VERTICE){
		this->v = createVertice(source.v->getType());
		this->v->importData(source.v->exportData());
	}
}
ldElement::ldElement()
{
	this->type	= EMPTY;
	this->v		= NULL;
}
ldElement::~ldElement()
{
	//----------------
	//	memory recycle
	//----------------
	if(this->v != NULL)
		delete this->v;
	this->v = NULL;
}

int ldElement::setType(unsigned char type)
{
	//-------------------------
	//	filter out unknown type
	//-------------------------
	if(type >= MAX_NUM_LD_ELEMENT)
		return 0;
		
	this->type = type;
	return 1;
}

int ldElement::setVertice(const int verticeType)
{
//	vertice	tmp;
	if(this->v != NULL){		// protection , Hsien , 2012.07.02
	//	tmp.importData(this->v->exportData());		// mode of data exchange , do not reset
		delete this->v;		// not good , using destructor would be mush more secure.
	}

	this->v = createVertice(verticeType);
	if(this->v == NULL)
		return 0;

//	this->v->importData(tmp.exportData());
	return 1;
}
int	ldElement::importData(const int _Code)
{
	int				mixedCode = _Code;
	unsigned char	*leCode,*verticeCode;

	leCode		= &(((unsigned char*)&mixedCode)[0]);
	verticeCode	= &(((unsigned char*)&mixedCode)[1]);

	if(!this->setType(*leCode))
		return 0;
	if(*leCode == VERTICE){
		if(!this->setVertice((const int)*verticeCode))
			return 0;
	}
	return 1;
}
int ldElement::importVerticeData(/*const int	mixedCode,*/const char*	data)
{
	//-------------------------------------------------------------
	//	rebuild ladder element following instrcution from data pack
	//-------------------------------------------------------------
	if(this->type != VERTICE)
		return 0;				// invalid for non-vertice element
	this->v->importData(data);
	return 1;
}
int			ldElement::exportData()
{
	//--------
	//
	//	Codec : 1th byte -> type of ladder element
	//			2th byte -> type of vertice , if any
	//--------
	int				code;
	unsigned char	*leCode,*verticeCode;

	leCode		= &(((unsigned char*)&code)[0]);
	verticeCode	= &(((unsigned char*)&code)[1]);

	*leCode = this->getType();
	if(this->type == VERTICE)
		*verticeCode	= this->v->getType();
	else
		*verticeCode	= 0;

	return code;
}
const char* ldElement::exportVerticeData(/*int			&code*/)
{
	if(this->type == VERTICE)
		return this->v->exportData();
	else
		return NULL;
}