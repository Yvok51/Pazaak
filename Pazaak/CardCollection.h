
#include <vector>
#include <memory>

#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/access.hpp>

#include "Card.h"
#include "GameConst.h"

#ifndef CARDCOLLECTION_H_
#define CARDCOLLECTION_H_

// Wrapper class for a vector of Cards with max size
class CardCollection {
	friend class Deck;
public:
	using CardVec = std::vector<std::unique_ptr<Card>>;

	CardCollection() : max_size_{ GameConst::TotalNumberOfCards } {}
	CardCollection(size_t max_size) : max_size_{ max_size } {};
	CardCollection(const CardCollection& collection) : max_size_{collection.max_size_} { clone(collection); }
	CardCollection(CardCollection&& collection) noexcept : max_size_{collection.max_size_}, collect_(std::move(collection.collect_)) {};
	CardCollection& operator=(const CardCollection& collection);
	CardCollection& operator=(CardCollection&& collection) noexcept;
	virtual ~CardCollection() noexcept = default;

	template <class	Archive>
	void serialize(Archive& archive); // Serialization method for cereal

	virtual void add(std::unique_ptr<Card> card);
	virtual void insert(std::unique_ptr<Card> card, size_t index);
	virtual void remove(size_t index);
	void clear() { collect_.clear(); }

	/* Has standard vector operator[] semantics - no bound checking */
	const std::unique_ptr<Card>& operator[](size_t index) const { return collect_[index]; };
	/* Has standard vector at() semantics - bound checking */
	const std::unique_ptr<Card>& at(size_t index) const { return collect_.at(index); };
	/* Has standard vector operator[] semantics - no bound checking */
	std::unique_ptr<Card>& operator[](size_t index) { return collect_[index]; };
	/* Has standard vector at() semantics - bound checking*/
	std::unique_ptr<Card>& at(size_t index) { return collect_.at(index); };

	CardVec::iterator begin() { return collect_.begin(); }
	CardVec::iterator end() { return collect_.end(); }
	CardVec::const_iterator begin() const { return collect_.begin(); }
	CardVec::const_iterator end() const { return collect_.end(); }
	CardVec::const_iterator cbegin() const { return collect_.cbegin(); }
	CardVec::const_iterator cend() const { return collect_.cbegin(); }

	std::unique_ptr<Card> copy(size_t index) const { return collect_[index]->clone(); };

	bool contains(const Card& card) const;
	bool isFull() const { return collect_.size() == max_size_; }
	size_t size() const { return collect_.size(); }
	size_t getMaxSize() const { return max_size_; }

protected:
	void clone(const CardCollection& collection);

	CardVec collect_;
	size_t max_size_;
};

template<class Archive>
inline void CardCollection::serialize(Archive& archive)
{
	archive(
		CEREAL_NVP(max_size_),
		CEREAL_NVP(collect_)
	);
}

#endif // !CARDCOLLECTION_H_