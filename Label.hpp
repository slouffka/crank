#ifndef CRANK_LABEL_HPP
#define CRANK_LABEL_HPP

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/Text.hpp>


namespace GUI
{

class Label : public Component
{
    public:
        typedef std::shared_ptr<Label> Ptr;


    public:
                            Label(const std::string& text, const FontManager& fonts);

        virtual bool        isSelectable() const;
        void                setText(const std::string& text);

        virtual void        handleEvent(const sf::Event& event);


    private:
        void                draw(sf::RenderTarget& target, sf::RenderStates states) const;


    private:
        sf::Text            mText;
};

}

#endif // CRANK_LABEL_HPP
