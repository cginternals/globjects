
#include <gmock/gmock.h>


#include <glow/ref_ptr.h>

class ref_ptr_test : public testing::Test
{
public:
};

class ReferencedMock : public glow::Referenced
{
public:
    MOCK_METHOD0(Die, void());
    virtual ~ReferencedMock() { Die(); };
};

TEST_F(ref_ptr_test, DeletesItsReferenced)
{
    // the ReferencedMock instance should be deleted on function exit

    glow::ref_ptr<ReferencedMock> ref = new ReferencedMock;
    EXPECT_CALL(*ref, Die()).Times(1);
}

TEST_F(ref_ptr_test, SurvivesWhenReferenced)
{
    glow::ref_ptr<ReferencedMock> owned;
    {
        glow::ref_ptr<ReferencedMock> ref = new ReferencedMock;
        EXPECT_CALL(*ref, Die()).Times(1);
        owned = ref;
        EXPECT_EQ(owned->refCounter(), 2);
    }
    EXPECT_EQ(owned->refCounter(), 1);
}

TEST_F(ref_ptr_test, ReferencesSameInstanceWhenPassedOn)
{
    glow::ref_ptr<ReferencedMock> ref = new ReferencedMock;
    EXPECT_CALL(*ref, Die()).Times(1);

    glow::ref_ptr<ReferencedMock> a = ref;
    glow::ref_ptr<ReferencedMock> b = ref;

    EXPECT_EQ(ref->refCounter(), 3);
    EXPECT_TRUE(a == b);
}
