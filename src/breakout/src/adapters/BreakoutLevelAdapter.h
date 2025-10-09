#pragma once

#include "ResourceAdapter.h"
#include "BreakoutLevel.h"
#include "JsonParser.h"

class BreakoutLevelAdapter: public ResourceAdapter {
public:
  BreakoutLevelAdapter() {
    logger = LoggerFactory::getLogger("video/BreakoutLevelAdapter");
    this->produces(BreakoutLevel::MimeType);
    this->accepts(MimeTypes::JSON);
  }

protected:
  virtual std::vector<Resource *> doLoad(ResourceLoadRequest &request) const override {
    std::vector<Resource *> response;

    JsonParser parser(request.getFileParser());
    unsigned int rows = 0, columns = 0;
    std::vector<unsigned int>bricks;

    String token;
    parser.readStartObject();
    while ((token = parser.readToken()) != END_OBJECT && token != FileParser::eof) {
      parser.readValueSeparator();

      if(token == "rows") {
        rows = parser.readInteger();
      } else if (token == "columns") {
        columns = parser.readInteger();
      } else if (token == "bricks") {
        bricks = parser.readUnsignedIntegerArray();
      } else {
        logger->error("Unexpected token: [%s] at (%d, %d)", token.c_str(), parser.getLine(), parser.getColumn());
      }

      if (parser.peekToken() == ",") {
        parser.readToken();
      }
    }

    if(rows > 0 && columns > 0) {
      if(rows * columns != bricks.size()) {
        logger->error("Rows, columns mistmatch bricks number");
      }

      BreakoutLevel *level = new BreakoutLevel(rows, columns);
      for(unsigned int i = 0; i < rows; i++) {
        for(unsigned int j = 0; j < columns; j++) {
          level->setBrick(i, j, bricks[i * columns + j]);
        }
      }

      response.push_back(level);
    }

    return response;
  }
};
