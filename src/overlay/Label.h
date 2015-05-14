/*
**	Author:		Martin Schwarz
**	Name:		Label.h
**	Project:	mgf - Mars Graphics Framework
*/

#ifndef MGF_LABEL_H
#define MGF_LABEL_H

#include "../Include.h"
#include "Moveable.h"
#include "../data/Texture.h"

namespace mgf{

class Label : public Moveable{
public:
	Label(const std::string &name);
	virtual ~Label();

	virtual bool render(std::shared_ptr<Renderer> renderer);

	virtual setText(const std::string &text);
	virtual setBackground(const std::string &path);
	virtual setBackground(std::shared_ptr<Texture> texture);

	virtual getText();
	virtual std::shared_ptr<Texture> getBackground();

protected:
	bool genBase();
	bool textToTexture(const std::string &text);

	std::string mText;
	std::shared_ptr<Texture> mTexture;
};

} // mgf

#endif // MGF_LABEL_H











