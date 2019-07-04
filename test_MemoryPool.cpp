#include <iostream>
#include "gtest/gtest.h"
#include "MemoryPool.h"

class IntClass {
    int *p_val;
public:
    IntClass() {
        p_val = new int(10);
    }

    int getValue() {
        return *p_val;
    }

    void setValue(int val) {
        *p_val = val;
    }

    virtual ~IntClass() {
        std::cout << "Destructor called for IntClass" << "\n";
        delete p_val;
    }
};

class StringClass {
    std::string *p_str;
public:
    StringClass() {
        p_str = new std::string("Initializer string\n");
    }

    std::string getValue() {
        return *p_str;
    }

    void setValue(std::string str) {
        *p_str = str;
    }

    ~StringClass() {
        std::cout << "Destructor called for StringClass" << "\n";
        delete p_str;
    }
};

class MemoryPoolTest : public ::testing::Test
{
public:
    MemoryPool<IntClass> pool{ 4 };
    IntClass *o1 = nullptr;

    void SetUp() override {
        o1 = pool.alloc();
    }

    void TearDown() override {
        pool.free(o1);
    }
};

TEST_F(MemoryPoolTest, testObjectType)
{
    EXPECT_EQ(typeid(*o1), typeid(IntClass));
}

TEST_F(MemoryPoolTest, testGetter)
{
    EXPECT_EQ(o1->getValue(), 10);
}

TEST_F(MemoryPoolTest, testSetter)
{
    o1->setValue(20);
    EXPECT_EQ(o1->getValue(), 20);
}

TEST_F(MemoryPoolTest, testMultipleObjectCreation)
{
    std::vector<IntClass*> vec;
    for(int k = 0; k < 10; ++k){
        auto* obj = pool.alloc();
        EXPECT_EQ(typeid(*obj), typeid(IntClass));
        vec.push_back(obj);
    }

    for(auto obj: vec){
        pool.free(obj);
    }
}

// #include "gmock/gmock.h"

// class MockIntClass: public IntClass{
// public:
//     MOCK_METHOD0(Die, void());
//     virtual ~MockIntClass() { Die(); }
// };

// TEST(MockTest, testDestructorCall)
// {
//     MockIntClass* foo = new MockIntClass;
//     {
//         EXPECT_CALL(*foo, Die());
//     }
// }