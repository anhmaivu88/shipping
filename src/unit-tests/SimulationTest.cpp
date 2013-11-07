#include "gtest/gtest.h"
#include "Simulation.h"
#include "FileParser.h"

class SimulationTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    simulation = Simulation::simulationNew("testSimulation");
    tissue = Tissue::TissueNew("testTissue");
    simulation->tissueIs(tissue);
  }

  Cell::Coordinates makeLocation(int x, int y, int z) {
    Cell::Coordinates location;
    location.x = x;
    location.y = y;
    location.z = z;
    return location;
  }

  Cell::Ptr buildChain(Cell::Coordinates location) {
    Cell::Ptr cell = FileParser::createCellAt(tissue, location, Cell::helperCell());
    FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("east"));
    FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("east"));
    FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("east"));
    
    return cell;
  }

  Cell::Ptr buildCube(Cell::Coordinates location) {
    Cell::Ptr cell = FileParser::createCellAt(tissue, location, Cell::helperCell());
    FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("east"));
    FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("east"));
    FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("north"));
    FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("north"));
    FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("up"));
    FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("up"));
    
    return cell;
  }

  Simulation::Ptr simulation;
  Tissue::Ptr tissue;
};

TEST_F(SimulationTest, CellCounterIncrementsAndDeletes) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  Cell::Coordinates location1 = makeLocation(0, 1, 0);
  Cell::Ptr cell1 = FileParser::createCellAt(tissue, location, Cell::helperCell());
  Cell::Ptr cell2 = FileParser::createCellAt(tissue, location1, Cell::cytotoxicCell());

  ASSERT_EQ(1, simulation->helperCells());
  ASSERT_EQ(1, simulation->cytotoxicCells());

  tissue->cellDel(cell1->name());
  tissue->cellDel(cell2->name());

  ASSERT_EQ(0, simulation->helperCells());
  ASSERT_EQ(0, simulation->cytotoxicCells());
}

TEST_F(SimulationTest, CellCounterCountsInfectedCopies) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  Cell::Ptr cell1 = FileParser::createCellAt(tissue, location, Cell::helperCell());
  cell1->healthIs(Cell::infected());
  // Not technically accurate but it's correct behavior in this program -- 
  // we operate under the assumption that we're the only infectors.
  ASSERT_EQ(0, simulation->infectedCells());
  FileParser::copyCellToOffset(cell1, Util::sideNameToCoordinateOffset("east"));
  ASSERT_EQ(1, simulation->infectedCells());
}

TEST_F(SimulationTest, MembraneConstructorBuildsMembranesWithAppropriateStrengths) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  Cell::Coordinates location1 = makeLocation(1, 0, 0);
  Cell::Ptr cell1 = FileParser::createCellAt(tissue, location, Cell::helperCell());
  Cell::Ptr cell2 = FileParser::createCellAt(tissue, location1, Cell::cytotoxicCell());

  ASSERT_NE((CellMembrane::Ptr) NULL, cell1->membrane(CellMembrane::up()));
  ASSERT_EQ(0, cell1->membrane(CellMembrane::up())->antibodyStrength().value());
  ASSERT_EQ(100, cell2->membrane(CellMembrane::up())->antibodyStrength().value());
}

TEST_F(SimulationTest, EmptyTissueSimulation) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  simulation->infectionIs(location, CellMembrane::east(), 100);

  ASSERT_EQ(0, simulation->helperCells());
  ASSERT_EQ(0, simulation->cytotoxicCells());
  ASSERT_EQ(0, simulation->infectionAttempts());
  ASSERT_EQ(0, simulation->strengthDifference());
  ASSERT_EQ(0, simulation->infectionPathLength());
  ASSERT_EQ(0, simulation->infectedCells());
  ASSERT_EQ(0, simulation->infectionVolume());
}

TEST_F(SimulationTest, SingleCellSuccessfulInfection) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  FileParser::createCellAt(tissue, location, Cell::helperCell());
  simulation->infectionIs(location, CellMembrane::east(), 100);
  
  ASSERT_EQ(100, simulation->strengthDifference());
  ASSERT_EQ(1, simulation->infectionPathLength());
  ASSERT_EQ(1, simulation->infectedCells());
  ASSERT_EQ(1, simulation->infectionVolume());
}

TEST_F(SimulationTest, SingleCellFailedInfectionDueToMembraneStrength) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  Cell::Ptr cell = FileParser::createCellAt(tissue, location, Cell::helperCell());
  FileParser::setMembraneStrengthOn(cell, CellMembrane::east(), 100);
  simulation->infectionIs(location, CellMembrane::east(), 50);
  
  ASSERT_EQ(-50, simulation->strengthDifference());
  ASSERT_EQ(0, simulation->infectionPathLength());
  ASSERT_EQ(0, simulation->infectedCells());
  ASSERT_EQ(0, simulation->infectionVolume());
  ASSERT_EQ(1, simulation->infectionAttempts());
}

TEST_F(SimulationTest, SingleCellFailedInfectionDueToAlreadyInfected) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  Cell::Ptr cell = FileParser::createCellAt(tissue, location, Cell::helperCell());
  cell->healthIs(Cell::infected());
  simulation->infectionIs(location, CellMembrane::east(), 50);
  
  ASSERT_EQ(0, simulation->strengthDifference());
  ASSERT_EQ(0, simulation->infectionPathLength());
  ASSERT_EQ(0, simulation->infectedCells());
  ASSERT_EQ(0, simulation->infectionVolume());
  ASSERT_EQ(0, simulation->infectionAttempts());
}

TEST_F(SimulationTest, TestSimpleChainInfection) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  // construct 4 cells starting at (0,0,0) in a line facing east. 
  Cell::Ptr cell = buildChain(location);

  simulation->infectionIs(location, CellMembrane::west(), 100);
  
  ASSERT_EQ(400, simulation->strengthDifference());
  ASSERT_EQ(4, simulation->infectionPathLength());
  ASSERT_EQ(4, simulation->infectedCells());
  ASSERT_EQ(4, simulation->infectionVolume());
}

TEST_F(SimulationTest, TestSimpleChainInfectionWithInfectedMiddle) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  // construct 4 cells starting at (0,0,0) in a line facing east. 
  Cell::Ptr cell = buildChain(location);
  Cell::Ptr infectedCell = tissue->cell(Util::CoordinateOffset(2, 0, 0) + location);
  infectedCell->healthIs(Cell::infected());

  // The infected cell should block the infection and not count as an attempt
  simulation->infectionIs(location, CellMembrane::west(), 100);
  
  ASSERT_EQ(200, simulation->strengthDifference());
  ASSERT_EQ(2, simulation->infectionPathLength());
  ASSERT_EQ(3, simulation->infectionVolume());
}

TEST_F(SimulationTest, TestSimpleChainInfectionWithMembraneMiddle) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  // construct 4 cells starting at (0,0,0) in a line facing east. 
  Cell::Ptr cell = buildChain(location);
  Cell::Ptr strongCell = tissue->cell(Util::CoordinateOffset(2, 0, 0) + location);
  strongCell->membrane(CellMembrane::west())->antibodyStrengthIs(100);

  // The membrane cell should block the infection
  simulation->infectionIs(location, CellMembrane::west(), 50);
  
  ASSERT_EQ(50, simulation->strengthDifference());
  ASSERT_EQ(2, simulation->infectionPathLength());
  ASSERT_EQ(2, simulation->infectionVolume());
}

TEST_F(SimulationTest, TextInfectionOfFilledCube) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  // construct 27 cell cube.
  Cell::Ptr cell = buildCube(location);

  simulation->infectionIs(location, CellMembrane::west(), 100);

  ASSERT_EQ(7, simulation->infectionPathLength());
  ASSERT_EQ(27, simulation->infectedCells());
  ASSERT_EQ(27, simulation->infectionVolume());
}

TEST_F(SimulationTest, TextInfectionOfHollowCube) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  // construct 27 cell cube.
  Cell::Ptr cell = buildCube(location);

  // remove the center; volume shouldn't change.
  Cell::Ptr centerCell = tissue->cell(Util::CoordinateOffset(1,1,1) + location);

  tissue->cellDel(centerCell->name());

  simulation->infectionIs(location, CellMembrane::west(), 100);

  ASSERT_EQ(7, simulation->infectionPathLength());
  ASSERT_EQ(26, simulation->infectedCells());
  ASSERT_EQ(27, simulation->infectionVolume());
}

TEST_F(SimulationTest, TestTwoSidedChainInfectionFromCenter) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  // construct 7 cells centered at (0,0,0) in a line.
  Cell::Ptr cell = buildChain(location);
  FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("west"));
  FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("west"));
  FileParser::cloneAllCellsToOffset(tissue, Util::sideNameToCoordinateOffset("west"));

  simulation->infectionIs(location, CellMembrane::west(), 100);
  
  ASSERT_EQ(4, simulation->infectionPathLength());
  ASSERT_EQ(7, simulation->infectedCells());
  ASSERT_EQ(7, simulation->infectionVolume());
}

TEST_F(SimulationTest, TestInfectionOnBothSidesOfMiddleMembrane) {
  Cell::Coordinates location = makeLocation(0, 0, 0);
  // construct 4 cells starting at (0,0,0) in a line facing east. 
  Cell::Ptr cell = buildChain(location);
  Cell::Ptr strongCell = tissue->cell(Util::CoordinateOffset(2, 0, 0) + location);
  strongCell->membrane(CellMembrane::west())->antibodyStrengthIs(100);

  // First we attack from the beginning of the chain
  simulation->infectionIs(location, CellMembrane::west(), 50);
  
  ASSERT_EQ(50, simulation->strengthDifference());
  ASSERT_EQ(2, simulation->infectionPathLength());
  ASSERT_EQ(2, simulation->infectedCells());
  ASSERT_EQ(2, simulation->infectionVolume());

  // Now we attack from the other side. We should infect everything.
  simulation->infectionIs(Util::CoordinateOffset(3, 0, 0) + location, CellMembrane::west(), 50);

  ASSERT_EQ(100, simulation->strengthDifference());
  ASSERT_EQ(2, simulation->infectionPathLength());
  ASSERT_EQ(4, simulation->infectedCells());
  ASSERT_EQ(4, simulation->infectionVolume());
}
