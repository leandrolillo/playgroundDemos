#pragma once

#include "Particle.h"

class SelectableParticle: public Particle {
private:
    bool _isSelected = false;

public:
    SelectableParticle(std::unique_ptr<Geometry> geometry) : Particle(std::move(geometry)) {
    }

    bool isSelected() const {
        return this->_isSelected;
    }

    void setSelected(bool selected) {
        this->_isSelected = selected;
    }
};
