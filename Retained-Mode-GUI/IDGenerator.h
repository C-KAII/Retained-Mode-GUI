#pragma once

class IDGenerator {
public:
  static IDGenerator& getInstance() {
    static IDGenerator instance;
    return instance;
  }

  int getNextID() {
    return ++m_currentID;
  }

private:
  int m_currentID{ 0 };

  IDGenerator() = default;
  IDGenerator(const IDGenerator&) = delete;
  IDGenerator& operator=(const IDGenerator&) = delete;
};

#define GEN_ID (IDGenerator::getInstance().getNextID())