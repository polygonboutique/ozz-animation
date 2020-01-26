//----------------------------------------------------------------------------//
//                                                                            //
// ozz-animation is hosted at http://github.com/guillaumeblanc/ozz-animation  //
// and distributed under the MIT License (MIT).                               //
//                                                                            //
// Copyright (c) 2019 Guillaume Blanc                                         //
//                                                                            //
// Permission is hereby granted, free of charge, to any person obtaining a    //
// copy of this software and associated documentation files (the "Software"), //
// to deal in the Software without restriction, including without limitation  //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,   //
// and/or sell copies of the Software, and to permit persons to whom the      //
// Software is furnished to do so, subject to the following conditions:       //
//                                                                            //
// The above copyright notice and this permission notice shall be included in //
// all copies or substantial portions of the Software.                        //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    //
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    //
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        //
// DEALINGS IN THE SOFTWARE.                                                  //
//                                                                            //
//----------------------------------------------------------------------------//

#include "ozz/base/memory/unique_ptr.h"

#include "ozz/base/gtest_helper.h"

TEST(Construction, UniquePtr) {
  { const ozz::UniquePtr<int> pi; }
  {
    const ozz::UniquePtr<int> pi(
        OZZ_NEW(ozz::memory::default_allocator(), int));
  }
}

TEST(Reset, UniquePtr) {
  {
    ozz::UniquePtr<int> pi;
    pi.reset(NULL);

    pi.reset(OZZ_NEW(ozz::memory::default_allocator(), int));
  }
  {
    ozz::UniquePtr<int> pi(OZZ_NEW(ozz::memory::default_allocator(), int));
    pi.reset(OZZ_NEW(ozz::memory::default_allocator(), int));
    pi.reset(NULL);
  }
  {
    int* i = OZZ_NEW(ozz::memory::default_allocator(), int)(46);
    ozz::UniquePtr<int> pi(i);
    EXPECT_ASSERTION(pi.reset(i),
                     "UniquePtr cannot be reseted to the same value.");
  }
}

struct A {
  int i;
};

TEST(Dereference, UniquePtr) {
  {
    const ozz::UniquePtr<int> pi;
    EXPECT_ASSERTION(*pi, "Dereferencing NULL pointer.");
    EXPECT_TRUE(pi.get() == NULL);
    EXPECT_FALSE(pi);
  }
  {
    const ozz::UniquePtr<int> pi(
        OZZ_NEW(ozz::memory::default_allocator(), int)(46));
    EXPECT_EQ(*pi, 46);
    EXPECT_TRUE(pi.get() != NULL);
    EXPECT_TRUE(pi);
  }
  {
    const ozz::UniquePtr<A> pa;
    EXPECT_ASSERTION(pa->i = 99, "Dereferencing NULL pointer.");
  }
  {
    const ozz::UniquePtr<A> pa(OZZ_NEW(ozz::memory::default_allocator(), A));
    pa->i = 46;
    EXPECT_EQ((*pa).i, 46);
  }
}

TEST(Bool, UniquePtr) {
  {
    const ozz::UniquePtr<int> pi;
    EXPECT_TRUE(!pi);
    EXPECT_FALSE(pi);
  }
  {
    const ozz::UniquePtr<int> pi(
        OZZ_NEW(ozz::memory::default_allocator(), int)(46));
    EXPECT_FALSE(!pi);
    EXPECT_TRUE(pi);
  }
}

TEST(Swap, UniquePtr) {
  {
    int* i = OZZ_NEW(ozz::memory::default_allocator(), int)(46);
    ozz::UniquePtr<int> pi;
    ozz::UniquePtr<int> pii(i);
    EXPECT_TRUE(pi.get() == NULL);
    EXPECT_TRUE(pii.get() == i);

    pi.swap(pii);
    EXPECT_TRUE(pii.get() == NULL);
    EXPECT_TRUE(pi.get() == i);
  }
  {
    int* i = OZZ_NEW(ozz::memory::default_allocator(), int)(46);
    ozz::UniquePtr<int> pi;
    ozz::UniquePtr<int> pii(i);
    EXPECT_TRUE(pi.get() == NULL);
    EXPECT_TRUE(pii.get() == i);

    swap(pi, pii);
    EXPECT_TRUE(pii.get() == NULL);
    EXPECT_TRUE(pi.get() == i);
  }
}

TEST(Release, UniquePtr) {
  {
    int* i = OZZ_NEW(ozz::memory::default_allocator(), int)(46);
    ozz::UniquePtr<int> pi(i);
    int* ri = pi.release();
    EXPECT_EQ(i, ri);
    OZZ_DELETE(ozz::memory::default_allocator(), ri);
  }
}

struct B : public A {};

TEST(Cast, UniquePtr) {
  ozz::UniquePtr<A> a = ozz::UniquePtr<B>();
  a = ozz::UniquePtr<B>();
}