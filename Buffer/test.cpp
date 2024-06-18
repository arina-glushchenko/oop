#include "pch.h"
#include "circular_buffer.h"

TEST(CircularBufferTest, DefaultConstructor) { //1 - default constructor
    CircularBuffer buffer;
    EXPECT_EQ(buffer.size(), 0);
    EXPECT_EQ(buffer.capacity(), 0);
    EXPECT_TRUE(buffer.empty());
    EXPECT_FALSE(buffer.full());
}

TEST(CircularBufferTest, IndexOperator) { 
    CircularBuffer buffer(3); //3 - buffer of a given capacity
    buffer.push_back('1'); //20 - push_back()
    buffer.push_back('2');
    buffer.push_back('3');

    EXPECT_EQ(buffer[0], '1'); //4 - operator[]()
    EXPECT_EQ(buffer[1], '2');
    EXPECT_EQ(buffer[2], '3');

    buffer[1] = '5';
    EXPECT_EQ(buffer.at(1), '5'); //5 - at()

    EXPECT_THROW(buffer.at(3), std::out_of_range);
}



TEST(CircularBufferTest, PushAndPop) {
    CircularBuffer buffer(3);

    buffer.push_back('1');
    buffer.push_back('2');
    buffer.push_front('3'); //21 - push_front()
    EXPECT_FALSE(buffer.empty()); //12 - empty()
    EXPECT_EQ(buffer.front(), '3'); //6 - front()
    EXPECT_EQ(buffer.back(), '2'); //7 - back()

    buffer.pop_front(); //23 - pop_front()
    EXPECT_EQ(buffer.front(), '1');
    buffer.pop_back(); //22 - pop_back()
    EXPECT_EQ(buffer.back(), '1');

    buffer.clear(); //26 - claer()
    EXPECT_EQ(buffer.size(), 0);
    EXPECT_EQ(buffer.capacity(), 3);


}


TEST(CircularBufferTest, CopyConstructorAndAssignmentOperator) {
    CircularBuffer buffer1(3);
    buffer1.push_back('1');
    buffer1.push_back('2');
    buffer1.push_back('3');

    CircularBuffer buffer2 = buffer1; //18 - operator=
    EXPECT_EQ(buffer2.size(), buffer1.size()); //11 - size()
    EXPECT_EQ(buffer2.capacity(), buffer1.capacity()); //15 - capacity()
    EXPECT_EQ(buffer2[0], buffer1[0]);
    EXPECT_EQ(buffer2[1], buffer1[1]);
    EXPECT_EQ(buffer2[2], buffer1[2]);

    CircularBuffer buffer3(5);
    buffer3 = buffer2; 
    EXPECT_EQ(buffer3.size(), buffer2.size());
    EXPECT_EQ(buffer3.capacity(), buffer2.capacity());
    EXPECT_EQ(buffer3[0], buffer2[0]);
    EXPECT_EQ(buffer3[1], buffer2[1]);
    EXPECT_EQ(buffer3[2], buffer2[2]);
}

TEST(CircularBufferTest, SwapAndEqualityOperators) {
    CircularBuffer buffer1(3);
    buffer1.push_back('1');
    buffer1.push_back('2');
    buffer1.push_back('3');

    CircularBuffer buffer2(3);
    buffer2.push_back('4');
    buffer2.push_back('5');
    buffer2.push_back('6');

    CircularBuffer buffer3 = buffer1;
    CircularBuffer buffer4 = buffer2;

    buffer1.swap(buffer2); //19 - swap()
    
    EXPECT_EQ(buffer1.size(), buffer4.size());
    EXPECT_EQ(buffer1.capacity(), buffer4.capacity());
    EXPECT_EQ(buffer1[0], buffer4[0]);
    EXPECT_EQ(buffer1[1], buffer4[1]);
    EXPECT_EQ(buffer1[2], buffer4[2]);

    EXPECT_EQ(buffer2.size(), buffer3.size());
    EXPECT_EQ(buffer2.capacity(), buffer3.capacity());
    EXPECT_EQ(buffer2[0], buffer3[0]);
    EXPECT_EQ(buffer2[1], buffer3[1]);
    EXPECT_EQ(buffer2[2], buffer3[2]);

    EXPECT_TRUE(buffer1==buffer4); //27 - operator==
    EXPECT_FALSE(buffer1==buffer3);
    EXPECT_TRUE(buffer2==buffer3);

    EXPECT_TRUE(buffer1!=buffer3); //28 - operator!=
    EXPECT_FALSE(buffer1!=buffer4);
    EXPECT_FALSE(buffer2!=buffer3);

    buffer3 = CircularBuffer(buffer1);
    EXPECT_TRUE(buffer1 == buffer3);

}

TEST(CircularBufferTest, InsertAndErase) {
    CircularBuffer buffer(3);
    buffer.push_back('1');
    buffer.push_back('2');
    buffer.push_back('3');

    EXPECT_EQ(buffer.reserve(), 0); //14 - reserve()
    buffer.set_capacity(4); //16 - set_capacity()
    EXPECT_EQ(buffer.capacity(), 4);
    EXPECT_EQ(buffer.reserve(), 1);
    EXPECT_THROW(buffer.set_capacity(2), std::length_error);

    buffer.insert(1, '5'); //24 - insert()
    EXPECT_EQ(buffer[0], '1');
    EXPECT_EQ(buffer[1], '5');
    EXPECT_EQ(buffer[2], '2');
    EXPECT_EQ(buffer[3], '3');
    EXPECT_TRUE(buffer.full()); //13 - full()


    buffer.erase(1, 2); //25 - erase()
    EXPECT_EQ(buffer[0], '1');
    EXPECT_EQ(buffer[1], '2');
    EXPECT_EQ(buffer.size(), 2);
    EXPECT_EQ(buffer.capacity(), 4);

    buffer.resize(3, '3'); //17 - resize()
    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer[2], '3');
    buffer.resize(1);
    EXPECT_EQ(buffer.size(), 1);
    EXPECT_EQ(buffer[0], '1');

}

TEST(CircularBufferTest, LinearizeAndRotate) {
    CircularBuffer buffer(4);
    buffer.push_back('1');
    buffer.push_back('2');
    buffer.push_back('3');
    buffer.push_back('4');

    value_type* linearizedBuffer = buffer.linearize(); //8 - linearize()
    EXPECT_TRUE(buffer.is_linearized()); //9 - is_linearized()


    EXPECT_EQ(*linearizedBuffer, '1');
    EXPECT_EQ(*(linearizedBuffer + 1), '2');
    EXPECT_EQ(*(linearizedBuffer + 2), '3');
    EXPECT_EQ(*(linearizedBuffer + 3), '4');

    buffer.rotate(2); //10 - rotate()
    EXPECT_EQ(buffer.front(), '3');
    EXPECT_EQ(buffer.back(), '2');
    EXPECT_FALSE(buffer.is_linearized());
}


//int main(int, char* []) {
//    CircularBuffer big;
//    CircularBuffer big2 = std::move(big);
//    std::cout << std::endl;
//    
//    
//    return 0;
//}
//
//
