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
        p_str = new std::string("Initializer String\n");
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
    MemoryPool<IntClass> pool1{ 4 };
    MemoryPool<StringClass> pool2{ 4 };
    
    IntClass *ptr1 = nullptr;
    StringClass *ptr2 = nullptr;

    void SetUp() override {
        ptr1 = pool1.alloc();
        ptr2 = pool2.alloc();
    }

    void TearDown() override {
        pool1.free(ptr1);
        pool2.free(ptr2);
    }
};

TEST_F(MemoryPoolTest, testObjectType)
{
    EXPECT_EQ(typeid(*ptr1), typeid(IntClass));
    EXPECT_EQ(typeid(*ptr2), typeid(StringClass));
}

TEST_F(MemoryPoolTest, testGetter)
{
    EXPECT_EQ(ptr1->getValue(), 10);
    EXPECT_EQ(ptr2->getValue(), "Initializer String\n");
}

TEST_F(MemoryPoolTest, testSetter)
{
    ptr1->setValue(20);
    EXPECT_EQ(ptr1->getValue(), 20);

    ptr2->setValue("Overwriting the Initialized String\n");
    EXPECT_EQ(ptr2->getValue(), "Overwriting the Initialized String\n");
}

TEST_F(MemoryPoolTest, testMultipleObjectCreation)
{
    {
        std::vector<IntClass*> vec;
        for(int k = 0; k < 10; ++k){
            auto* obj = pool1.alloc();
            EXPECT_EQ(typeid(*obj), typeid(IntClass));
            vec.push_back(obj);
        }

        for(auto obj: vec){
            pool1.free(obj);
        }
    }
    {
        std::vector<StringClass*> vec;
        for(int k = 0; k < 10; ++k){
            auto* obj = pool2.alloc();
            EXPECT_EQ(typeid(*obj), typeid(StringClass));
            vec.push_back(obj);
        }

        for(auto obj: vec){
            pool2.free(obj);
        }
    }
}