#include "gtest/gtest.h"
#include "FileParser.h"

class FileParserTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    parser = FileParser::newParser("testParser");
    tissue = Tissue::TissueNew("testTissue");
    parser->tissueIs("testTissue", tissue);
  }

  FileParser::Ptr parser;
  Tissue::Ptr tissue;
  
  Cell::Coordinates makeLocation(int x, int y, int z) {
    Cell::Coordinates location;
    location.x = x;
    location.y = y;
    location.z = z;
    return location;
  }
};

// ----- testing the string interface below ---- //
TEST_F(FileParserTest, CreateTissue) {
  parser->lineIs(0, "Tissue tissueNew firstTissue");
  ASSERT_NE(parser->tissue("firstTissue"), (Tissue::Ptr) NULL);
  ASSERT_EQ(parser->tissue("noTissueHere"), (Tissue::Ptr) NULL);
}

TEST_F(FileParserTest, CreateCytoCell) {
  parser->lineIs(0, "Tissue testTissue cytotoxicCellNew 0 0 1");
  Tissue::Ptr tissue = parser->tissue("testTissue");
  Cell::Coordinates location = makeLocation(0, 0, 1);
  Cell::Coordinates location2 = makeLocation(0, 0, 0);

  ASSERT_NE(tissue->cell(location), (Cell::Ptr) NULL);
  ASSERT_EQ(tissue->cell(location2), (Cell::Ptr) NULL);

  ASSERT_EQ(tissue->cell(location)->cellType(), Cell::cytotoxicCell());
}

TEST_F(FileParserTest, CreateHelperCell) {
  parser->lineIs(0, "Tissue testTissue helperCellNew 0 0 1");
  Tissue::Ptr tissue = parser->tissue("testTissue");
  Cell::Coordinates location = makeLocation(0, 0, 1);
  Cell::Coordinates location2 = makeLocation(0, 0, 0);

  ASSERT_NE(tissue->cell(location), (Cell::Ptr) NULL);
  ASSERT_EQ(tissue->cell(location2), (Cell::Ptr) NULL);

  ASSERT_EQ(tissue->cell(location)->cellType(), Cell::helperCell());
}

TEST_F(FileParserTest, SingleCellClone) {
  Tissue::Ptr tissue = parser->tissue("testTissue");

  Cell::Coordinates location = makeLocation(0, 0, 0);
  Cell::Coordinates location2 = makeLocation(0, 0, 1);

  FileParser::createCellAt(tissue, location, Cell::helperCell());

  parser->lineIs(0, "Cell testTissue 0 0 0 cloneNew up");


  ASSERT_NE(tissue->cell(location), (Cell::Ptr) NULL);
  ASSERT_NE(tissue->cell(location2), (Cell::Ptr) NULL);
}

TEST_F(FileParserTest, MultiCellClone) {
  Tissue::Ptr tissue = parser->tissue("testTissue");
  Cell::Coordinates location = makeLocation(0, 0, 0);
  Cell::Coordinates location2 = makeLocation(0, 1, 0);
  Cell::Coordinates location3 = makeLocation(1, 0, 0);
  Cell::Coordinates location4 = makeLocation(1, 1, 0);

  FileParser::createCellAt(tissue, location, Cell::helperCell());
  FileParser::createCellAt(tissue, location3, Cell::helperCell());

  parser->lineIs(0, "Tissue testTissue cloneCellsNew north");


  ASSERT_NE(tissue->cell(location), (Cell::Ptr) NULL);
  ASSERT_NE(tissue->cell(location2), (Cell::Ptr) NULL);
  ASSERT_NE(tissue->cell(location3), (Cell::Ptr) NULL);
  ASSERT_NE(tissue->cell(location4), (Cell::Ptr) NULL);
}

TEST_F(FileParserTest, DelInfectedCells) {
  Tissue::Ptr tissue = parser->tissue("testTissue");

  Cell::Coordinates location = makeLocation(0, 0, 0);

  FileParser::createCellAt(tissue, location, Cell::helperCell());

  Cell::Ptr cell = tissue->cell(location);
  cell->healthIs(Cell::infected());

  parser->lineIs(0, "Tissue testTissue infectedCellsDel");

  ASSERT_EQ((Cell::Ptr) NULL, tissue->cell(location));
}

TEST_F(FileParserTest, SetAntibodyStrength) {
  Tissue::Ptr tissue = parser->tissue("testTissue");
  Cell::Coordinates location = makeLocation(0, 0, 0);
  FileParser::createCellAt(tissue, location, Cell::helperCell());

  parser->lineIs(0, "Cell testTissue 0 0 0 membrane east antibodyStrengthIs 50");

  ASSERT_EQ(50, tissue->cell(location)->membrane(CellMembrane::east())->antibodyStrength().value());
}

TEST_F(FileParserTest, InfectionStartLocation) {
  Tissue::Ptr tissue = parser->tissue("testTissue");
  Cell::Coordinates location = makeLocation(0, 1, 0);
  FileParser::createCellAt(tissue, location, Cell::helperCell());
  parser->lineIs(0, "Tissue testTissue infectionStartLocationIs 0 1 0 east 99");

  ASSERT_EQ(Cell::infected(), tissue->cell(location)->health());
}

TEST_F(FileParserTest, IgnoresCommandsAddressedToNonExistentTissue) {
  ASSERT_NO_THROW(
                  parser->lineIs(0, "Tissue testTissue3 infectionStartLocationIs 0 1 0 east 99");
                  );
}

TEST_F(FileParserTest, IgnoresCommandsAddressedToNonExistentCell) {
  ASSERT_NO_THROW(
                  parser->lineIs(0, "Cell testTissue 0 0 0 membrane east antibodyStrengthIs 50");
                  );
}

// ----- testing the static methods that actually implement the above commands now. ------ //
// arguably we don't need to do this, but I'll test more complicated edge cases now. //

TEST_F(FileParserTest, CopyOverOtherCellShouldAbortAndNotThrow) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  Cell::Coordinates location2 = makeLocation(0, 1, 0);
  Cell::Ptr cell = FileParser::createCellAt(tissue, location, Cell::helperCell());
  FileParser::createCellAt(tissue, location2, Cell::cytotoxicCell());
  Cell::Ptr copiedCell;
  ASSERT_NO_THROW(
                  copiedCell = FileParser::copyCellToOffset(cell, Util::sideNameToCoordinateOffset("north"))
                  );

  ASSERT_EQ(Cell::cytotoxicCell(), tissue->cell(location2)->cellType());
  ASSERT_EQ((Cell::Ptr) NULL, copiedCell);
}

TEST_F(FileParserTest, CreateCellAtOccupiedLocationShouldDoNothing) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  FileParser::createCellAt(tissue, location, Cell::helperCell());
  Cell::Ptr secondCell;
  ASSERT_NO_THROW(
                  secondCell = FileParser::createCellAt(tissue, location, Cell::cytotoxicCell());
                  );

  ASSERT_EQ(Cell::helperCell(), tissue->cell(location)->cellType());
  ASSERT_EQ((Cell::Ptr) NULL, secondCell);
}
