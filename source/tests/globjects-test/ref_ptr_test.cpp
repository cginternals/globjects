
#include <gmock/gmock.h>


#include <globjects/base/ref_ptr.h>

class ref_ptr_test : public testing::Test
{
public:
};

class ReferencedMock : public globjects::Referenced
{
public:
    MOCK_METHOD0(Die, void());
    virtual ~ReferencedMock() { Die(); }
};

TEST_F(ref_ptr_test, DeletesItsReferenced)
{
    // the ReferencedMock instance should be deleted on function exit

    globjects::ref_ptr<ReferencedMock> ref = new ReferencedMock;
    EXPECT_CALL(*ref, Die()).Times(1);
}

TEST_F(ref_ptr_test, SurvivesWhenReferenced)
{
    globjects::ref_ptr<ReferencedMock> owned;
    {
        globjects::ref_ptr<ReferencedMock> ref = new ReferencedMock;
        EXPECT_CALL(*ref, Die()).Times(1);
        owned = ref;
        EXPECT_EQ(owned->refCounter(), 2);
    }
    EXPECT_EQ(owned->refCounter(), 1);
}

TEST_F(ref_ptr_test, ReferencesSameInstanceWhenPassedOn)
{
    globjects::ref_ptr<ReferencedMock> ref = new ReferencedMock;
    EXPECT_CALL(*ref, Die()).Times(1);

    globjects::ref_ptr<ReferencedMock> a = ref;
    globjects::ref_ptr<ReferencedMock> b = ref;

    EXPECT_EQ(ref->refCounter(), 3);
    EXPECT_TRUE(a == b);
}

TEST_F(ref_ptr_test, ReferenceComparisonConstComparisonsCompile)
{
    SUCCEED(); // Just test compilation, not actual behavior

    ReferencedMock * t = new ReferencedMock;
    const ReferencedMock * const_t = new ReferencedMock;

    globjects::ref_ptr<ReferencedMock> ref_t = new ReferencedMock;
    const globjects::ref_ptr<ReferencedMock> const_ref_t = new ReferencedMock;
    globjects::ref_ptr<const ReferencedMock> ref_const_t = new ReferencedMock;
    const globjects::ref_ptr<const ReferencedMock> const_ref_const_t = new ReferencedMock;

    bool success1 = true;
    bool success2 = true;

    if (t == ref_t || t == const_ref_t || t == ref_const_t || t == const_ref_const_t)
        if (const_t == ref_t || const_t == const_ref_t || const_t == ref_const_t || const_t == const_ref_const_t)
            if (ref_t == const_ref_t || ref_t == ref_const_t || ref_t == const_ref_const_t)
                if (const_ref_t == ref_const_t || const_ref_t == const_ref_const_t)
                    if (ref_const_t == const_ref_const_t)
                    {
                        success1 = false;
                    }

    if (ref_t == t || const_ref_t == t || ref_const_t == t || const_ref_const_t == t)
        if (ref_t == const_t || const_ref_t == const_t || ref_const_t == const_t || const_ref_const_t == const_t)
            if (const_ref_t == ref_t || ref_const_t == ref_t || const_ref_const_t == ref_t)
                if (ref_const_t == const_ref_t || const_ref_const_t == const_ref_t)
                    if (const_ref_const_t == ref_const_t)
                    {
                        success2 = false;
                    }

    EXPECT_TRUE(success1 && success2);

    delete t;
    delete const_t;
}
