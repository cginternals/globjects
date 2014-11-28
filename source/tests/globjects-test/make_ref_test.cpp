
#include <gmock/gmock.h>


#include <globjects/base/ref_ptr.h>

class make_ref_test : public testing::Test
{
public:
};

namespace
{
class ReferencedMock : public globjects::Referenced
{
public:
    int m_i;
    float m_j;

    ReferencedMock(int i, float j):
        m_i(i),
        m_j(j)
    {     
    }

    ReferencedMock(float && j, int && i):
        m_i(i),
        m_j(j)
    {     
    }

    ReferencedMock():
        m_i(13),
        m_j(3.7f)
    {     
    }
};
}

TEST_F(make_ref_test, CallableByValue)
{
    globjects::ref_ptr<ReferencedMock> ref = globjects::make_ref<ReferencedMock>(5, 3.0f);

    EXPECT_EQ(ref->m_i, 5);
    EXPECT_FLOAT_EQ(ref->m_j, 3.0f);
}

TEST_F(make_ref_test, CallableByRValue)
{
    globjects::ref_ptr<ReferencedMock> ref = globjects::make_ref<ReferencedMock>(2.3f, 6);

    EXPECT_EQ(ref->m_i, 6);
    EXPECT_FLOAT_EQ(ref->m_j, 2.3f);
}

TEST_F(make_ref_test, CallableWithoutParams)
{
    globjects::ref_ptr<ReferencedMock> ref = globjects::make_ref<ReferencedMock>();

    EXPECT_EQ(ref->m_i, 13);
    EXPECT_FLOAT_EQ(ref->m_j, 3.7f);
}

TEST_F(make_ref_test, CorrectRefCount)
{
    globjects::ref_ptr<ReferencedMock> ref = globjects::make_ref<ReferencedMock>();

    EXPECT_EQ(ref->refCounter(), 1);
}
