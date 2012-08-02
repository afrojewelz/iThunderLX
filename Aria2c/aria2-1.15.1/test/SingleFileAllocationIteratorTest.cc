#include "SingleFileAllocationIterator.h"
#include "File.h"
#include "DefaultDiskWriter.h"
#include <fstream>
#include <cppunit/extensions/HelperMacros.h>

namespace aria2 {

class SingleFileAllocationIteratorTest:public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(SingleFileAllocationIteratorTest);
  CPPUNIT_TEST(testAllocate);
  CPPUNIT_TEST_SUITE_END();
private:

public:
  void setUp() {}

  void testAllocate();
};


CPPUNIT_TEST_SUITE_REGISTRATION( SingleFileAllocationIteratorTest );

void SingleFileAllocationIteratorTest::testAllocate()
{
  std::string dir = A2_TEST_OUT_DIR;
  std::string fname = "aria2_SingleFileAllocationIteratorTest_testAllocate";
  std::string fn = dir+"/"+fname;
  std::ofstream of(fn.c_str(), std::ios::binary);
  of << "0123456789";
  of.close();

  File x(fn);
  CPPUNIT_ASSERT_EQUAL((off_t)10, x.size());

  DefaultDiskWriter writer(fn);
  off_t offset = 10;
  off_t totalLength = 16*1024*2+8*1024;

  // we have to open file first.
  writer.openExistingFile();
  SingleFileAllocationIterator itr(&writer, offset, totalLength);
  itr.init();

  while(!itr.finished()) {
    itr.allocateChunk();
  }
  File f(fn);
  CPPUNIT_ASSERT_EQUAL((off_t)40960, f.size());
}

} // namespace aria2
