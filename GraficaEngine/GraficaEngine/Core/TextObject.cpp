#include "TextObject.h"
#include "Scene.h"
#include "Canvas.h"

namespace Engine {

	TextObject::TextObject(std::string text) 
    {
        _text = text;
        _font = FontManager::getInstance()->getFont("arial_rounded.TTF"); //TODO add optional parameter with default font
	}

    TextObject::TextObject(const TextObject* otherTextObject) :
        _font(otherTextObject->_font),
        _text(otherTextObject->_text),
        _color(otherTextObject->_color),
        BaseGameObject(otherTextObject)
    {
    }

    TextObject *TextObject::clone() const
    {
        return new TextObject(this);
    }

	void TextObject::draw() const
    {
        Shader* shader = FontManager::getInstance()->getFontsShader();
        shader->use();
        shader->setVec3f("textColor", glm::value_ptr(_color));

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(_font.VAO);

        // Acumulate position in X axis so we display each char next to the previous one
        float positionX = transform.position.x;

        _parent->transform.apply(*shader, "parentModel");
        transform.apply(*shader);

        std::string::const_iterator c;
        for (char const& c : _text)
        {
            const Engine::Character ch = _font.characters.at(c);

            shader->setFloat("positionX", positionX);
            shader->setFloat("positionY", transform.position.y);

            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ch.EBO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            positionX += (ch.Advance >> 6) * transform.scale.x; // bitshift by 6 to get value in pixels (2^6 = 64)
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
	}

    bool TextObject::isDrawable() const
    {
        return true;
    }

    void TextObject::setText(std::string text)
    {
        _text = text;
    }

    void TextObject::setFont(std::string font)
    {
        _font = FontManager::getInstance()->getFont(font);
    }

    void TextObject::setColor(glm::vec3 color)
    {
        _color = color;
    }
}
