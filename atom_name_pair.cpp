#include "atom_name_pair.h"

bool atom_name_pair::operator==(const atom_name_pair& other) const {
	return (this->center == other.center || this->center == other.reference) &&
	       (this->reference == other.center || this->reference == other.reference);
}