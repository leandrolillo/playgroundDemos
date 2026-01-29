#pragma once

#include "Resource.h"
#include <Math3d.h>

class BreakoutLevel: public Resource {
private:
  matriz_mxn bricks;
public:
  inline static const String MimeType = "breakout/level";

public:
  BreakoutLevel(unsigned int rows, unsigned int columns) : Resource(MimeType), bricks(rows, columns) {
  }

  unsigned int getRows() const {
    return bricks.getNroFilas();
  }
  unsigned int getColumns() const {
    return bricks.getNroColumnas();
  }

  void setBrick(unsigned int row, unsigned int column, unsigned int health) {
    bricks(row, column) = health;
  }

  unsigned int getBrickAt(unsigned int row, unsigned int column) const {
    return bricks(row, column);
  }
};
