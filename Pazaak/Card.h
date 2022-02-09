
#include <string>
#include <memory>

#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/access.hpp>

#ifndef CARD_H_
#define CARD_H_

enum class CardEffect {
    None,
    Double,
    TwoAndFour,
    ThreeAndSix,
    Tiebraker
};

enum class Color {
    None,
    Green,
    Blue,
    Red,
    Yellow,
    BlueAndRed
};

class Card {
    friend class cereal::access;

public:
    virtual std::unique_ptr<Card> clone() const = 0;

    bool operator==(const Card& rhs) const;

    // Getters
    const std::string& getText() const { return text_; }
    int getValue() const { return value_; }
    Color getColor() const { return color_; }
    virtual CardEffect getSpecialEffect() const { return CardEffect::None; }
    virtual bool isFlipable() const { return false; };
    virtual bool usable() const { return true; }

    // Modifiers
    void flipValue() { value_ = -value_; updateText(); }
    void doubleValue() { value_ = 2 * value_; updateText(); }
    virtual void updateText() { text_ = std::to_string(value_); }

protected:
    virtual bool isEqual(const Card& rhs) const = 0;

    int value_{ 0 };
    std::string text_{""};
    Color color_{ Color::Blue };

private:
    template <class	Archive> // cereal serialization function
    void serialize(Archive& archive) { archive(CEREAL_NVP(value_), CEREAL_NVP(text_), CEREAL_NVP(color_)); }
};

class HandCard final : public Card {
    friend class cereal::access;

public:
    HandCard();
    HandCard(int value);
    virtual std::unique_ptr<Card> clone() const override { return std::make_unique<HandCard>(*this); }

private:
    template <class	Archive> // cereal serialization function
    void serialize(Archive& archive) { archive(cereal::base_class<Card>(this)); }

    virtual bool isEqual(const Card& rhs) const override { return value_ == static_cast<const HandCard&>(rhs).value_; }

    static constexpr int Min = -6;
    static constexpr int Max = 6;
};

class DealerCard final : public Card {
    friend class cereal::access;

public:
    DealerCard();
    DealerCard(int value);
    virtual std::unique_ptr<Card> clone() const override { return std::make_unique<DealerCard>(*this); }

private:
    template <class	Archive> // cereal serialization function
    void serialize(Archive& archive) { archive(cereal::base_class<Card>(this)); }


    virtual bool isEqual(const Card& rhs) const override { return value_ == static_cast<const DealerCard&>(rhs).value_; }

    static constexpr int Min = 1;
    static constexpr int Max = 10;
};

class FlipableCard final : public Card {
    friend class cereal::access;

public:
    FlipableCard();
    FlipableCard(int value);
    bool isFlipable() const override { return true; }
    virtual std::unique_ptr<Card> clone() const override { return std::make_unique<FlipableCard>(*this); }

private:
    template <class	Archive> // cereal serialization function
    void serialize(Archive& archive) { archive(cereal::base_class<Card>(this)); }

    virtual bool isEqual(const Card& rhs) const override { return value_ == static_cast<const FlipableCard&>(rhs).value_; }

    static constexpr int Min = -6;
    static constexpr int Max = 6;
};

class ThreeAndSixCard final : public Card {
    friend class cereal::access;

public:
    ThreeAndSixCard();
    virtual CardEffect getSpecialEffect() const override { return CardEffect::ThreeAndSix; }
    virtual std::unique_ptr<Card> clone() const override { return std::make_unique<ThreeAndSixCard>(*this); }

private:
    template <class	Archive> // cereal serialization function
    void serialize(Archive& archive) { archive(cereal::base_class<Card>(this)); }

    virtual bool isEqual(const Card& rhs) const override { return value_ == static_cast<const ThreeAndSixCard&>(rhs).value_; }

};

class TwoAndFourCard final : public Card {
    friend class cereal::access;

public:
    TwoAndFourCard();
    virtual CardEffect getSpecialEffect() const override { return CardEffect::TwoAndFour; }
    virtual std::unique_ptr<Card> clone() const override { return std::make_unique<TwoAndFourCard>(*this); }

private:
    template <class	Archive> // cereal serialization function
    void serialize(Archive& archive) { archive(cereal::base_class<Card>(this)); }

    virtual bool isEqual(const Card& rhs) const override { return value_ == static_cast<const TwoAndFourCard&>(rhs).value_; }

};

class DoubleCard final : public Card {
    friend class cereal::access;

public:
    DoubleCard();
    virtual CardEffect getSpecialEffect() const override { return CardEffect::Double; }
    virtual std::unique_ptr<Card> clone() const override { return std::make_unique<DoubleCard>(*this); }

private:
    template <class	Archive> // cereal serialization function
    void serialize(Archive& archive) { archive(cereal::base_class<Card>(this)); }

    virtual bool isEqual(const Card& rhs) const override { return value_ == static_cast<const DoubleCard&>(rhs).value_; }

};

class TiebrakerCard final : public Card {
    friend class cereal::access;

public:
    TiebrakerCard();
    bool isFlipable() const override { return true; }
    virtual CardEffect getSpecialEffect() const override { return CardEffect::Tiebraker; }
    virtual void updateText() override { if (value_ > 0) { text_ = "+" + std::to_string(value_) + "T"; } else { text_ = std::to_string(value_) + "T"; } }
    virtual std::unique_ptr<Card> clone() const override { return std::make_unique<TiebrakerCard>(*this); }

private:
    template <class	Archive> // cereal serialization function
    void serialize(Archive& archive) { archive(cereal::base_class<Card>(this)); }

    virtual bool isEqual(const Card& rhs) const override { return value_ == static_cast<const TiebrakerCard&>(rhs).value_; }
};

class EmptyCard final : public Card {
    friend class cereal::access;

public:
    EmptyCard();
    virtual std::unique_ptr<Card> clone() const override { return std::make_unique<EmptyCard>(*this); }
    virtual void updateText() override {};
    virtual bool usable() const override { return false; }

private:
    template <class	Archive> // cereal serialization function
    void serialize(Archive& archive) { archive(cereal::base_class<Card>(this)); }

    virtual bool isEqual(const Card& rhs) const override { return value_ == static_cast<const EmptyCard&>(rhs).value_; }

};

// Cereal register polymorphic classes
CEREAL_REGISTER_TYPE(HandCard);
CEREAL_REGISTER_TYPE(DealerCard);
CEREAL_REGISTER_TYPE(FlipableCard);
CEREAL_REGISTER_TYPE(ThreeAndSixCard);
CEREAL_REGISTER_TYPE(TwoAndFourCard);
CEREAL_REGISTER_TYPE(DoubleCard);
CEREAL_REGISTER_TYPE(TiebrakerCard);
CEREAL_REGISTER_TYPE(EmptyCard);

#endif // !CARD_H_

