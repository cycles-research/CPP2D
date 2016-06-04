#include <unordered_map>
#include "framework.h"


struct ContainHashMap
{
	std::unordered_map<int, int> m1;
};

struct ContainHashMap2
{
	std::unordered_map<int, int> m1;

	ContainHashMap2() = default;

	ContainHashMap2(ContainHashMap2 const& other)
		: m1(other.m1)
	{
	}
};

struct ContainHashMap3
{
	std::unordered_map<int, int> m1;

	ContainHashMap3() = default;

	ContainHashMap3(ContainHashMap3 const& other)
	{
		m1 = other.m1;
	}
};


void check_std_unordered_map()
{
	std::unordered_map<int, int> m1;
	m1[36] = 78;
	CHECK(m1[36] == 78);

	std::vector<int> v1;
	std::vector<int> v2;
	for (auto i : m1)
	{
		v1.push_back(i.first);
		v2.push_back(i.second);
	}
	CHECK(v1[0] == 36);
	CHECK(v2[0] == 78);

	{
		ContainHashMap c1;
		c1.m1[15] = 98;
		auto c2 = c1;
		c1.m1[15] = 6;
		CHECK(c2.m1[15] == 98);
		c2 = c1;
		c1.m1[15] = 7;
		CHECK(c2.m1[15] == 6);
	}

	{
		ContainHashMap2 c1;
		c1.m1[15] = 98;
		auto c2 = c1;
		c1.m1[15] = 6;
		CHECK(c2.m1[15] == 98);
		c2 = c1;
		c1.m1[15] = 7;
		CHECK(c2.m1[15] == 6);
	}

	{
		ContainHashMap3 c1;
		c1.m1[15] = 98;
		auto c2 = c1;
		c1.m1[15] = 6;
		CHECK(c2.m1[15] == 98);
		c2 = c1;
		c1.m1[15] = 7;
		CHECK(c2.m1[15] == 6);
	}
}

class Class781
{
public:
	int i = 0;
	Class781() {};
	Class781(int u) :i(u) {};
};

struct Struct781
{
	int i = 0;
	Struct781() = default;
	Struct781(int u) :i(u) {};
};

void check_shared_ptr()
{
	std::shared_ptr<Class781> classptr1;  //default ctor
	CHECK(classptr1 == nullptr);
	classptr1.reset(new Class781());
	CHECK(classptr1 != nullptr);
	std::shared_ptr<Class781> classptr2 = classptr1;  //copy ctor
	CHECK(classptr1 == classptr2);
	//std::shared_ptr<Class781> classptr3(new Class781()); //Use make_shared!!
	//CHECK(classptr3 != nullptr);
	std::shared_ptr<Class781> classptr4 = std::make_shared<Class781>();  //make_shared ctor
	CHECK(classptr4 != nullptr);
	classptr4 = std::make_shared<Class781>(); //assign make_shared
	CHECK(classptr4 != nullptr);
	classptr4 = classptr2; //assign
	CHECK(classptr4 == classptr2);

	std::shared_ptr<Struct781> structptr1; //default ctor
	CHECK(structptr1 == nullptr);
	structptr1 = std::make_shared<Struct781>(); //assign make_shared
	CHECK(structptr1 != nullptr);
	//structptr1.reset(new Struct781()); //Use make_shared!!
	//CHECK(structptr1 != nullptr);
	std::shared_ptr<Struct781> structptr2 = structptr1; //copy stor
	CHECK(structptr2 == structptr1);
	//std::shared_ptr<Struct781> structptr3(new Struct781()); //Use make_shared!!
	//CHECK(structptr2 != nullptr);
	std::shared_ptr<Struct781> structptr4 = std::make_shared<Struct781>(); //make_shared ctor
	CHECK(structptr4 != nullptr);
	structptr4 = structptr1; //assign
	CHECK(structptr4 == structptr1);
	return;
}

void check_unique_ptr()
{
	std::unique_ptr<Class781> classptr1;  //default ctor
	CHECK(classptr1 == nullptr);
	classptr1.reset(new Class781());
	CHECK(classptr1 != nullptr);
	std::unique_ptr<Class781> classptr2 = std::move(classptr1);  //move copy
	CHECK(classptr2 != nullptr);
	CHECK(classptr1 == nullptr);
	//std::unique_ptrd_ptr<Class781> classptr3(new Class781()); //Use make_shared!!
	//CHECK(classptr3 != nullptr);
	std::unique_ptr<Class781> classptr4 = std::make_unique<Class781>();  //make_shared ctor
	CHECK(classptr4 != nullptr);
	classptr4 = std::make_unique<Class781>(); //assign make_shared
	CHECK(classptr4 != nullptr);
	classptr4 = std::move(classptr2); //move assign
	CHECK(classptr4 != nullptr);
	CHECK(classptr2 == nullptr);

	std::unique_ptr<Struct781> structptr1; //default ctor
	CHECK(structptr1 == nullptr);
	structptr1 = std::make_unique<Struct781>(); //assign make_shared
	CHECK(structptr1 != nullptr);
	//structptr1.reset(new Struct781()); //Use make_shared!!
	//CHECK(structptr1 != nullptr);
	std::unique_ptr<Struct781> structptr2 = std::move(structptr1); //move copy
	CHECK(structptr2 != nullptr);
	CHECK(structptr1 == nullptr);
	//std::unique_ptr<Struct781> structptr3(new Struct781()); //Use make_shared!!
	//CHECK(structptr2 != nullptr);
	std::unique_ptr<Struct781> structptr4 = std::make_unique<Struct781>(); //make_shared ctor
	CHECK(structptr4 != nullptr);
	structptr4 = std::move(structptr2); //assign
	CHECK(structptr4 != nullptr);
	CHECK(structptr2 == nullptr);
	return;
}

void stdlib_register(TestFrameWork& tf)
{
	auto ts = std::make_unique<TestSuite>();

	ts->addTestCase(check_std_unordered_map);

	ts->addTestCase(check_shared_ptr);

	ts->addTestCase(check_unique_ptr);

	tf.addTestSuite(std::move(ts));
}