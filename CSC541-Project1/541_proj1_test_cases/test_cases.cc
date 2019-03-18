#include "gtest/gtest.h"
#include "API.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// tests empty text
TEST(test_create_text, create_test){
    text_t* text = create_text();
    ASSERT_NE(text, (text_t*)NULL);
}

// tests if create_text returns length 0
TEST(test_create_text, new_text_length_test){
    text_t* text = create_text();
    ASSERT_EQ(length_text(text), 0);
}

// tests if null input is correctly handled
TEST(test_length_text, null_input_test){
    ASSERT_LT(length_text(NULL), 0);
}

// tests if line was appended correclty to the end of an empty text
TEST(test_length_text, one_line_test){
    text_t* text = create_text();
    append_line(text, "foobar");
    ASSERT_EQ(length_text(text), 1);
}

// tests long text
TEST(test_length_text, k_lines_test){
    text_t* text = create_text();
    for (int i=0; i<1000; i++)
    {
        append_line(text, "foobar");
    }
    ASSERT_EQ(length_text(text), 1000);
}

//test whether the implementation is balanced search tree (running time will be O(log n))
TEST(test_length_text, k_lines_test_1){
    text_t* text = create_text();
    for (long int i=0; i<10000000; i++)
    {
        append_line(text, "foobar");
        //cout << "\n appended line i = " << i << endl;
    }
    EXPECT_EQ(length_text(text), 10000000);
}

// multiple tests
TEST(test_1, _){
    int i;
    text_t* text1 = create_text();
    append_line(text1, "line one" );
    append_line(text1, "line hundred" );
    insert_line(text1, 2, "line ninetynine" );
    insert_line(text1, 2, "line ninetyeight" );
    insert_line(text1, 2, "line ninetyseven" );
    insert_line(text1, 2, "line ninetysix" );
    insert_line(text1, 2, "line ninetyfive" );
    for( i = 2; i <95; i++ )
        insert_line(text1, 2, "some filler line between 1 and 95" );
    EXPECT_EQ((length_text(text1)), 100) << ("Length should be 100, instead is %d\n", length_text(text1));

    
    EXPECT_STREQ(get_line(text1,  2), "some filler line between 1 and 95" ) << "Wrong sentence at line 2";
    EXPECT_STREQ(get_line(text1,  99), "line ninetynine" ) << "Wrong sentence at line 99";
    EXPECT_STREQ(get_line(text1,  100), "line hundred" ) << "Wrong sentence at line 100";

    set_line(text1, 100, "the last line");
    for( i=99; i>=1; i-- )
        delete_line(text1, i );
    ASSERT_STREQ((get_line(text1,  1)), "the last line") << ("The last line should be /the last line/, instead is %d\n", get_line(text1,  1));
}

// multiple tests
TEST(test_2, _){
    int i, tmp; 
    text_t *txt2; 
    char *c;


    txt2 = create_text();
    for(i=1; i<=10000; i++){  
        if( i%2==1 )
            append_line(txt2, "A");
        else 
            append_line(txt2, "B");
    }

    tmp = length_text(txt2);
    EXPECT_EQ(tmp,10000) << ("length should be 10000, is %d\n", tmp) ;
    
    c = get_line(txt2, 9876 );
    EXPECT_EQ(*c,'B') << ("line 9876 of txt2 should be B, found %s\n", c);

    for( i= 10000; i > 1; i-=2 ){  
        c = delete_line(txt2, i);
        EXPECT_EQ(*c,'B') << ("line %d of txt2 should be B, found %s\n", i, c);
        append_line( txt2, c );
   }
    for( i=1; i<= 5000; i++ ){
        c = get_line(txt2, i);
        EXPECT_EQ(*c,'A') << ("line %d of txt2 should be A, found %s\n", i, c);
    }


    for( i=1; i<= 5000; i++ ){
        delete_line(txt2, 1 );
    }

    for( i=1; i<= 5000; i++ ){
        c = get_line(txt2, i);
        EXPECT_EQ(*c,'B') << ("line %d of txt2 should be B, found %s\n", i, c);
    }

    set_line(txt2, 100, "the last line");
    for( i=99; i>=1; i-- ){
        delete_line(txt2, i );
    }
    ASSERT_STREQ(get_line(txt2,  1), "the last line") << ("found at the last line:   %s\n", get_line(txt2,  1));
}






