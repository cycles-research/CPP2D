#include "test.h"
#include "framework.h"
#include <cstdarg>
#include <cstdio>

#define CHECK(COND) check(COND, #COND, __LINE__) //if(!(COND)) error(#COND)

size_t equal_len(char const* a, char const* b)
{
	size_t pos = 0;
	while (*a == *b && *a != 0 && *b != 0)
	{
		++a;
		++b;
		++pos;
	}
	return pos;
}

int find_string(char const* str, char const* sub)
{
	int pos = 0;
	while (str[pos] != 0)
	{
		size_t len = equal_len(str + pos, sub);
		if (sub[len] == 0)
			return pos;
		++pos;
	}
	return -1;
}

bool cmp_string(char const* a, char const* b)
{
	size_t pos = equal_len(a, b);
	return a[pos] == b[pos];
}


class Tata
{
public:
	virtual int funcV()
	{
		return 1;
	}

	int funcF()
	{
		return 1;
	}
};

class Tata2 : public Tata
{
public:
	int value;

	Tata2(int arg = 13)
	{
		value = arg;
	}

	int funcV() override
	{
		return 2;
	}

	int funcF(int i = 0)
	{
		return 2;
	}

	Tata2* getThis()
	{
		return this;
	}

	Tata2& getThisRef()
	{
		return *this;
	}
};

template<int I>
class TmplClass1
{
public:
	static int const value = I;
};

template<int I>
class TmplClass2 : public TmplClass1<I> {};

template<>
class TmplClass2<0> : public TmplClass1<42> {};

template<typename T1, typename T2>
bool fl_equal(T1 v1, T2 v2)
{
	auto diff = v1 - v2;
	return (diff < 0. ? -diff : diff) < 0.00001;
}

void check_operators()
{
	//**************************** operators **********************************
	//    Math
	CHECK(45 == 45);
	CHECK(11 != 88);
	CHECK(3 + 2 == 5);
	CHECK(3 - 2 == 1);
	CHECK(3 * 2 == 6);
	CHECK(6 / 2 == 3);

	int u = 0;
	u += 2;
	CHECK(u == 2);
	u -= 1;
	CHECK(u == 1);
	u *= 6;
	CHECK(u == 6);
	u /= 3;
	CHECK(u == 2);

	//    Bin
	CHECK(true && true);
	CHECK(true || false);

	// bitwise
	CHECK((0x3 & 0x1) == 0x1);
	CHECK((0x3 | 0x1) == 0x3);
	CHECK((0x3 ^ 0x1) == 0x2);
}

int func0() { return 3; }

int func10(int i = 42) { return i; }

void func12(int& i, int u) { i = 42; u = 43; }

int z = 45;

int& func15() { return z; }

const int& func16() { return z; }

void check_function()
{
	//*********************** Functions ***************************************
	CHECK(func0() == 3);

	CHECK(func10(7) == 7);

	CHECK(func10() == 42);

	int a = 0, b = 0;
	func12(a, b);
	CHECK(a == 42 && b == 0);

	func15() = 98;
	CHECK(z == 98);
	CHECK(&func15() == &z);
}

void check_static_array()
{
	int tab[3] = { 1, 2, 3 };
	tab[1] = 42;
	CHECK(tab[1] == 42);
	CHECK(tab[2] == 3);

	int tab3[] = { 4, 5, 6, 7 };
	CHECK(sizeof(tab3) == 4 * sizeof(int));

	int tabA[3] = { 1, 2, 3 };
	int tabB[3] = { 1, 2, 3 };
	CHECK(tabA != tabB);  // Pointer comparison

	typedef int start[];

	start bli = { 4, 8, 9 };
	CHECK(bli[2] == 9);
}

//! To ensure the same function isn't print twice
void check_dynamic_array();

void check_dynamic_array()
{
	int *tab2 = new int[46];
	tab2[6] = 42;
	CHECK(tab2[6] == 42);
	delete[] tab2;
	tab2 = nullptr;

	int *tabC = new int[3];
	int *tabD = new int[3];
	for (int i = 0; i < 3; ++i)
	{
		tabC[i] = i - 1;
		tabD[i] = i - 1;
	}
	CHECK(tabD != tabC); // Pointer comparison

	tabD = tabC;   // Copy pointer
	tabC[1] = 78;
	CHECK(tabD == tabC); // Pointer comparison
}

struct Toto3
{
	int a;
	float b;
	bool c;
	static short u;

	int getA() const
	{
		return a;
	}

	void setA(int a_)
	{
		a = a_;
	}

	static int getStatic()
	{
		return 12;
	}

	bool operator==(Toto3 const& other) const
	{
		return a == other.a && b == other.b && c == other.c;
	}

	//virtual void impossible(){}
};

short Toto3::u = 36;


void check_struct()
{
	//! comment Toto
	struct Toto
	{
		int a;
		float b;
		bool c;
	};
	Toto toto; //defult ctor
	toto.a = 6;
	// test commment
	CHECK(toto.a == 6);

	Toto toto2 = { 7, 8.99f, true };
	CHECK(toto2.a == 7 && fl_equal(toto2.b, 8.99) && toto2.c == true);

	//! comment Toto2
	struct Toto2
	{
		//! comment a
		int a;
		float b; //!< comment b
		bool c;
		Toto2(int a_,
			float b_,
			bool c_ //!< comment c
			)
			: a(a_)
			, b(b_)
			, c(c_)
		{
		}

		Toto2* getThis()
		{
			return this;
		}

		Toto2& getThisRef()
		{
			return *this;
		}
	};

	Toto2 toto3(18, 12.57f, true);
	CHECK(toto3.a == 18 && fl_equal(toto3.b, 12.57) && toto3.c == true);
	CHECK(toto3.getThis() == &toto3);
	CHECK(&toto3.getThisRef() == &toto3);

	Toto3 t3;
	t3.setA(16);
	CHECK(t3.getA() == 16);
	CHECK(Toto3::getStatic() == 12);

	Toto3 const t4 = { 16 };
	CHECK(t4.getA() == 16);

	Toto3::u = 17;
	CHECK(Toto3::u == 17);

	t3.b = 0.00795f;
	Toto3 t5 = t3;
	CHECK(&t5 != &t3);  // Pointer compatison
	CHECK(t5 == t3);   //Content comparison
};

// Template type
template<typename T, int I>
T funcT()
{
	return T(I);
}

// Template type deduction
template<typename T>
T funcT2(T a, int mult)
{
	return a * mult;
}

template<typename T1, typename T2>
struct is_same
{
	static bool const value = false;
};

template<typename T1>
struct is_same<T1, T1>
{
	static bool const value = true;
};

void check_template_struct_specialization()
{
	bool b = is_same<int, int>::value;
	CHECK(b);
	b = is_same<int, float>::value;
	CHECK(!b);
}

void check_template_function()
{
	auto res1 = funcT<int, 4>();
	const bool isInt = is_same<decltype(res1), int>::value;
	CHECK(isInt);
	CHECK(res1 == 4);
	auto res2 = funcT2(3, 2);
	CHECK(res2 == 6);
}


void check_class()
{
	Tata2* t2 = new Tata2();
	CHECK(t2->funcV() == 2);
	CHECK(t2->funcF() == 2);
	Tata* t = t2;
	CHECK(t->funcV() == 2);
	CHECK(t->funcF() == 1);
	CHECK(t2->getThis() == t2);
	CHECK(t2 == t);
	CHECK(&t2->getThisRef() == t2);
	delete t;
	t = nullptr;
	CHECK(t == nullptr);

	CHECK(TmplClass2<3>::value == 3);
	CHECK(TmplClass2<0>::value == 42);

	Tata2 t3;
	CHECK(&t3 != nullptr);
	CHECK(t3.value == 13);

	Tata2 t4(54);
	CHECK(&t4 != nullptr);
	CHECK(t4.value == 54);
}

template<typename A, typename B>
struct same_type;

template<typename A>
struct same_type<A, A> { static bool const value = true; };

template <typename T> struct A { typedef T type; };
template <typename T> using Type = typename A<T>::type;

void check_type_alias()
{
	using type = int;
	static_assert(same_type<type, int>::value, "same_type<type, int>::value");

	static_assert(same_type<Type<int>, A<int>::type>::value, "same_type<type, int>::value");
}

struct HasBitField
{
	unsigned char a : 1, b : 2, c : 3;
};

struct HasBitField2
{
	int a : 7;
	int b : 8;
	int c;
};

void check_bitfield()
{
	static_assert(sizeof(HasBitField) == 1, "sizeof(HasBitField) == 1");
	static_assert(sizeof(HasBitField2) == 8, "sizeof(HasBitField2) == 8");

	HasBitField bf; // = { 0, 1, 3 }; // Do not work in D
	bf.a = 0;
	bf.b = 1;
	bf.c = 3;
	CHECK(bf.a == 0 && bf.b == 1 && bf.c == 3);
	bf.a = 1;
	CHECK(bf.a == 1 && bf.b == 1 && bf.c == 3);
	bf.b = 2;
	CHECK(bf.a == 1 && bf.b == 2 && bf.c == 3);
	bf.c = 2;
	CHECK(bf.a == 1 && bf.b == 2 && bf.c == 2);
}

int decayed_type_func(int a[2], int b[], int(c)(int))
{
	return c(a[0] + a[1] + b[3]);
}

int mult2(int val)
{
	return val * 2;
}

void check_decayed_type()
{
	int a[2] = { 1, 2 };
	int b[4] = { 0, 1, 2, 4 };
	CHECK(decayed_type_func(a, b, mult2) == 14);
}

template <typename T> struct InjectedClassNameType
{
public:
	InjectedClassNameType():data(2) {  }
	T data;
	InjectedClassNameType const& toto(InjectedClassNameType const* a)
	{
		return *a;
	}
};

void check_injectedclassnametype()
{
	InjectedClassNameType<int const> toto;
	InjectedClassNameType<int const> toto2;
	CHECK(&toto.toto(&toto2) == &toto2);
}


class ConvertibleClass
{
	int a_;
public:
	ConvertibleClass(int a) :a_(a) {}

	operator int()
	{
		return a_;
	}
};

struct ConvertibleStruct
{
	int a_;

	ConvertibleStruct(int a) :a_(a) {}

	operator int()
	{
		return a_;
	}
};

void check_convertion_operator()
{
	ConvertibleClass a(7);
	CHECK(a == 7);

	ConvertibleStruct b(19);
	CHECK(b == 19);
}

class AbsMethClass
{
public:
	virtual ~AbsMethClass() = 0 {};
	virtual int meth() = 0;
};

class AbsMethClass2 : public AbsMethClass
{
	int meth()
	{
		return 12;
	}
};

void check_abstract_keyword()
{
	AbsMethClass* a = new AbsMethClass2();
	CHECK(a->meth() == 12);
	delete a;
}


void check_struct_opassign()
{
	struct S
	{
		int a;
		int b;

		S& operator = (S const& other)
		{
			a = other.a;
			return *this;
		}
	};

	S a = { 1, 2 };
	S const b = { 3, 4 };
	a = b;
	CHECK(a.a == 3 && a.b == 2);
}

void check_class_opassign()
{
	class S
	{
	public:
		int a;
		int b;
		S(int a_, int b_) :a(a_), b(b_) {}

		S& operator = (int c)
		{
			a = c;
			return *this;
		}
	};

	S a(1, 2);
	a = 59;
	CHECK(a.a == 59 && a.b == 2);

	S *c = new S(1, 2);
	*c = 47;
	CHECK(c->a == 47);
	CHECK(c->b == 2);
	delete c;
}


template<typename A>
bool is_int()
{
	return false;
}

template<>
bool is_int<int>()
{
	return true;
}

void check_tmpl_func_spec()
{
	struct Blip {};
	CHECK(is_int<Blip>() == false);
	CHECK(is_int<int>() == true);
}

void var_func(char* buffer, const char* fmt, ...)
{
	//! TODO : Waiting for macro handling
	/*va_list args;
	va_start(args, fmt);

	while (*fmt != '\0') {
		if (*fmt == 'd') {
			int i = va_arg(args, int);
			buffer += sprintf(buffer, "%d ", i);
		}
		else if (*fmt == 'c') {
			// note automatic conversion to integral type
			int c = va_arg(args, int);
			buffer += sprintf(buffer, "%c ", c);
		}
		else if (*fmt == 'f') {
			double d = va_arg(args, double);
			buffer += sprintf(buffer, "%f ", d);
		}
		++fmt;
	}

	va_end(args);*/
	return;
}


void check_variadic()
{
	char buffer[1000];
	var_func(buffer, "dcff", 3, 'a', 1.999, 42.5);
	//CHECK(cmp_string("3 a 1.999000 42.500000 ", buffer));
}

template<int I>
struct TmplMethstruct2
{
	int a;
};


template<int I>
struct TmplMethstruct
{
	int a = I;

	TmplMethstruct() :a(I) {}

	template<int I2>
	TmplMethstruct(TmplMethstruct<I2> const&) : a(I2) {}
	
	template<int I2>
	operator TmplMethstruct2<I2>()
	{
		TmplMethstruct2<I2> bli = {a};
		return bli;
	}

	template<int I2>
	TmplMethstruct& operator = (TmplMethstruct<I2> const& ot)
	{
		a = ot.a;
		return *this;
	}

	template<int I2>
	TmplMethstruct operator - (TmplMethstruct<I2> const& ot)
	{
		TmplMethstruct res;
		res.a = a - ot.a;
		return res;
	}

	template<int I2>
	bool operator < (TmplMethstruct<I2> const& ot)
	{
		return a < ot.a;
	}

};

void check_tmpl_meth()
{
	TmplMethstruct<4> a;
	TmplMethstruct<7> b(a);
	CHECK(a.a == 4); //default ctor
	CHECK(b.a == 4); //copy ctor
	b.a = 56;
	b = a;  // copy operator
	CHECK(b.a == 4);
	b.a = 56;

	TmplMethstruct2<78> c = b; //cast operator
	CHECK(c.a == 56);

	auto d = b - a;
	CHECK(d.a == 52); // binary operator

	//CHECK(a < d); // comparison
}

int tata(short a) { return a * 2; }

int toto(short a) { return a * 2; }

void check_function_pointer()
{
	int(*a)(short) = tata;
	int(*b)(short) = toto;
	CHECK(a != b);
	CHECK(a(2) == b(2));
	a = b;
	CHECK(a == b);
}


void check_method_pointer()
{
	struct MethPointerStruct
	{
		int tata(short a) { return a * 2; }
		int toto(short a) { return a * 2; }
	};
	/*MethPointerStruct toto;
	int (MethPointerStruct::*a)(short) = &MethPointerStruct::tata;
	int (MethPointerStruct::*b)(short) = &MethPointerStruct::toto;
	CHECK(a != b);
	CHECK((toto.*a)(2) == (toto.*b)(2));
	a = b;
	CHECK(a == b);
	*/
	return;
}


void check_member_pointer()
{
	struct MemPointerStruct
	{
		int a = 1;
		int b = 2;
	} instance;

	/*int MemPointerStruct::* i = &MemPointerStruct::a;
	int MemPointerStruct::* j = &MemPointerStruct::b;
	CHECK(i != j);
	
	j = &MemPointerStruct::b;
	CHECK((instance.*i) == 1);
	CHECK((instance.*j) == 2);
	i = j;
	CHECK(i == j);
	CHECK((instance.*i) == 2);*/
	return;
}

enum Empty {};

struct FriendStruct1;
struct FriendStruct2
{
	friend FriendStruct1;
};
struct FriendStruct1
{
	friend FriendStruct2;
};


struct CheckUDAArg
{
	typedef int A;
	static int const a = 42;
};

template<typename T>
struct CheckUDA
{
	typedef typename T::A T_A;
	T_A a = T::a;
	T_A func(T_A a = T::a)
	{
		return a;
	};
};

void check_uninstantiated_default_arg()
{
	CheckUDA<CheckUDAArg> a;
	CHECK(a.a == 42);
	CHECK(a.func() == 42);
}

void check_class_instantiation()
{
	class StackClass
	{
	public:
		int a;

		StackClass(int a_ = 0) : a(a_) {}
	};
	StackClass a;
	StackClass b(1);
	StackClass c = 2;

	StackClass *d = new StackClass;
	StackClass *e = new StackClass(1);
	CHECK(d->a != e->a);
	CHECK(d != e);
	delete d;

	d = e;
	CHECK(d == e);
	delete e;
}

typedef int incomp_arr[];
int func_incomp_arr(incomp_arr arr)
{
	return arr[3];
}

void check_incomplete_array_type()
{
	int toto[] = { 0, 1, 2, 3 };
	CHECK(func_incomp_arr(toto) == 3);
}


template<typename T>
struct UninstantiatedStruct
{
	template<typename I>
	typename T::A func(typename I::B a)
	{
		typename I::C c;
		typename I::D d;
		return c + d.z - I::C::y;
	}
};

void check_builtin_macro()
{
	auto l = __LINE__;
	CHECK(find_string(__FILE__, "test") != -1);
	CHECK(find_string(__FUNCTION__, "check_builtin_macro") != -1);
	CHECK(find_string(__func__, "check_builtin_macro") != -1);
	CHECK(__LINE__ == l + 4);
}

void check_incr_pointer()
{
	size_t const SIZE = 10;
	int tab[SIZE];
	int val = 0;
	for (int* ptr = tab; ptr != tab + 10; ++ptr, ++val) // preincr
		*ptr = val;
	int* ptr = tab;
	ptr++;					//postincr
	ptr += 2;				// +=
	int* ptr2 = ptr + 2;	//+
	CHECK(*ptr2 == 5);
}

#define UT_MACRO(expr1, expr2, expr3)  \
	(var##expr1 = 42, var2 = #expr2, var3 = expr3, expr1 + expr3)

#define UT_MACRO_STMT  \
int a = 1; \
int b = 2;

#define UT_MACRO_STMT_CLASS(P, T, N, V) P: T N = V;

class MacroClass
{
	UT_MACRO_STMT_CLASS(public, int, a, 1);
	UT_MACRO_STMT_CLASS(public, int, b, 2);
};

template<int I>
struct MPInt {};

int forward(int i)
{
	return i;
}

#define UT_MACRO_EXPR(A, B) A + B

void check_function_macro()
{
	int var1(0);
	char const* var2 = nullptr;
	int var3 = 0;
	int var4(UT_MACRO(1, 2, 3)); 

	CHECK(var1 == 42);
	CHECK(var2[0] == '2');
	CHECK(var3 == 3);
	CHECK(var4 == 4);

	UT_MACRO_STMT

	CHECK(a == 1 && b == 2);

	MacroClass* m = new MacroClass();
	CHECK(m->a == 1 && m->b == 2);
	delete m;

	CHECK(forward(UT_MACRO_EXPR(4, 3)) == 7);
}

void check_range_based_for_loop()
{
	int tab[] = { 1, 2, 3 };
	int sum = 0;
	for (int i : tab)
		sum += i;
	CHECK(sum == 6);
	
	sum = 0;
	for (auto i : tab)
		sum += i;
	CHECK(sum == 6);

	for (int& i : tab)
		i = i + 1;
	CHECK(tab[0] == 2 && tab[1] == 3 && tab[2] == 4);

	for (auto& i : tab)
		i = i + 1;
	CHECK(tab[0] == 3 && tab[1] == 4 && tab[2] == 5);

	int ctab[] = { 1, 2, 3 };
	sum = 0;
	for (auto const& i : ctab)
		sum += i;
	CHECK(sum == 6);

}

struct OverloadOpStuct2
{
	int value;
};

struct OverloadOpStuct
{
	int value = 0;

	explicit OverloadOpStuct(int v) : value(v) {};

	// Unary operators
	OverloadOpStuct operator -() const { return OverloadOpStuct(-value); }
	OverloadOpStuct operator +() const { return OverloadOpStuct(+value); }
	OverloadOpStuct operator *() const { return OverloadOpStuct(value * value); }
	OverloadOpStuct& operator ++()
	{
		++value;
		return *this;
	}
	OverloadOpStuct& operator --()
	{
		--value;
		return *this;
	}
	OverloadOpStuct operator ++(int)
	{
		OverloadOpStuct tmp(*this);
		++(*this);
		return tmp;
	}
	OverloadOpStuct operator --(int)
	{
		OverloadOpStuct tmp(*this);
		--(*this);
		return tmp;
	}

	// Cast operator
	operator int() const {return 42;}

	//*************** Binary operators ************************************************************

	// Arithmetic operators

	OverloadOpStuct& operator += (OverloadOpStuct const& other)
	{
		value += other.value;
		return *this;
	}

	OverloadOpStuct operator + (OverloadOpStuct const& other) const
	{
		OverloadOpStuct tmp(*this);
		return tmp += other;
	}

	OverloadOpStuct& operator -= (OverloadOpStuct const& other)
	{
		value -= other.value;
		return *this;
	}

	OverloadOpStuct operator - (OverloadOpStuct const& other) const
	{
		OverloadOpStuct tmp(*this);
		return tmp -= other;
	}

	OverloadOpStuct& operator *= (OverloadOpStuct const& other)
	{
		value *= other.value;
		return *this;
	}

	OverloadOpStuct operator * (OverloadOpStuct const& other) const
	{
		OverloadOpStuct tmp(*this);
		return tmp *= other;
	}

	OverloadOpStuct& operator /= (OverloadOpStuct const& other)
	{
		value /= other.value;
		return *this;
	}

	OverloadOpStuct operator / (OverloadOpStuct const& other) const
	{
		OverloadOpStuct tmp(*this);
		return tmp /= other;
	}

	// relational operators

	bool operator == (OverloadOpStuct const& other) const
	{
		return value == other.value;
	}

	bool operator != (OverloadOpStuct const& other) const
	{
		return !(*this == other);
	}

	bool operator < (OverloadOpStuct const& other) const
	{
		return (this->value < other.value);
	}

	bool operator > (OverloadOpStuct const& other) const
	{
		return (this->value > other.value);
	}

	bool operator <= (OverloadOpStuct const& other) const
	{
		return (*this < other) || (*this == other);
	}

	bool operator >= (OverloadOpStuct const& other) const
	{
		return (this->value >= other.value);
	}

	// asymetric relational operators
	bool operator == (OverloadOpStuct2 const& other) const
	{
		return value == other.value;
	}

	bool operator != (OverloadOpStuct2 const& other) const
	{
		return !(*this == other);
	}

	bool operator < (OverloadOpStuct2 const& other) const
	{
		return (this->value < other.value);
	}

	bool operator > (OverloadOpStuct2 const& other) const
	{
		return (this->value > other.value);
	}

	bool operator <= (OverloadOpStuct2 const& other) const
	{
		return (*this < other) || (*this == other);
	}

	bool operator >= (OverloadOpStuct2 const& other) const
	{
		return (this->value >= other.value);
	}

	//************************ logical operators **************************************************
	bool operator ! () const
	{
		return this->value == 0;
	}

	bool operator && (OverloadOpStuct const& other) const
	{
		return (!!*this) && (!!other);
	}

	bool operator || (OverloadOpStuct const& other) const
	{
		return (!!*this) || (!!other);
	}

	//************************ Bitwise operators **************************************************
	OverloadOpStuct operator ~ () const
	{
		return OverloadOpStuct(~(this->value));
	}

	OverloadOpStuct operator & (OverloadOpStuct const& other) const
	{
		return OverloadOpStuct(this->value & other.value);
	}

	OverloadOpStuct operator | (OverloadOpStuct const& other) const
	{
		return OverloadOpStuct(this->value | other.value);
	}

	OverloadOpStuct operator ^ (OverloadOpStuct const& other) const
	{
		return OverloadOpStuct(this->value ^ other.value);
	}

	OverloadOpStuct operator << (int count) const
	{
		return OverloadOpStuct(this->value << count);
	}

	OverloadOpStuct operator >> (int count) const
	{
		return OverloadOpStuct(this->value >> count);
	}

	// Member and pointer operators
	int operator [] (int arg) const
	{
		return value + arg;
	}

	int const* operator & () const
	{
		return &value;
	}

	int* operator & ()
	{
		return &value;
	}

	OverloadOpStuct* operator -> ()
	{
		return this;
	}

	int operator()(int a, int b)
	{
		return a + b;
	}

	int operator,(int a)
	{
		return a * 3;
	}
};

void check_overloaded_operator()
{
	//****************   Unary operators  *********************************************************
	{
		OverloadOpStuct i(18);
		CHECK((-i).value == -18);
		CHECK((+i).value == +18);
		CHECK((~i).value == ~18);
		CHECK((*i).value == 18 * 18);
		// Pre incr and decr
		CHECK((++i).value == 19);
		CHECK(i.value == 19);
		CHECK((--i).value == 18);
		CHECK(i.value == 18);
		// post incr and decr
		CHECK((i++).value == 18);
		CHECK(i.value == 19);
		CHECK((i--).value == 19);
		CHECK(i.value == 18);
	}

	//*************************** Cast operator ***************************************************
	{
		OverloadOpStuct i(18);
		CHECK((int)i == 42);
	}

	//********************* Binary operators ******************************************************
	// Arithmetic operators
	{
		OverloadOpStuct a(6);
		OverloadOpStuct b(3);

		OverloadOpStuct mu = a * b;
		CHECK(mu.value == 18);

		OverloadOpStuct d = a / b;
		CHECK(d.value == 2);

		OverloadOpStuct p = a + b;
		CHECK(p.value == 9);

		OverloadOpStuct mi = a - b;
		CHECK(mi.value == 3);

		mu /= b;
		CHECK(mu.value == a.value);

		d *= b;
		CHECK(d.value == a.value);

		p -= b;
		CHECK(p.value == a.value);

		mi += b;
		CHECK(mi.value == a.value);
	}

	// relational operators
	{
		OverloadOpStuct a6(6);
		OverloadOpStuct b6(6);
		OverloadOpStuct c3(3);
		CHECK(b6 == a6);
		CHECK(b6 <= a6);
		CHECK(b6 >= a6);
		CHECK(c3 != a6);
		CHECK(c3  < a6);
		CHECK(c3 <= a6);
		CHECK(a6  > c3);
		CHECK(a6 >= c3);
	}

	// Asymetric relational operators
	{
		OverloadOpStuct2 a6 = { 6 };
		OverloadOpStuct b6(6);
		OverloadOpStuct c3(3);
		OverloadOpStuct d9(9);
		CHECK(b6 == a6);
		CHECK(b6 <= a6);
		CHECK(b6 >= a6);
		CHECK(c3 != a6);
		CHECK(c3  < a6);
		CHECK(c3 <= a6);
		CHECK(d9  > a6);
		CHECK(d9 >= a6);
	}

	// Logical operators
	{
		OverloadOpStuct a0(0);
		OverloadOpStuct b0(0);
		OverloadOpStuct c1(48);
		OverloadOpStuct d1(79);
		CHECK(!a0);
		CHECK(!!c1);
		CHECK(!(a0 && b0));
		CHECK(!(a0 && c1));
		CHECK(!(c1 && a0));
		CHECK(c1 && d1);
		CHECK(!(a0 || b0));
		CHECK(a0 || c1);
		CHECK(c1 || a0);
		CHECK(c1 || d1);
	}

	//************************ Bitwise operators **************************************************
	{
		auto inv = ~decltype(OverloadOpStuct::value)(0);
		OverloadOpStuct a000(0);
		OverloadOpStuct a110(6);
		OverloadOpStuct a011(3);
		CHECK((~a000).value == inv);
		CHECK((a110 & a011) == OverloadOpStuct(2));
		CHECK((a110 | a011) == OverloadOpStuct(7));
		CHECK((a110 ^ a011) == OverloadOpStuct(5));
		CHECK((a110 >> 1) == OverloadOpStuct(3));
		CHECK((a110 << 1) == OverloadOpStuct(12));
	}

	// Member and pointer operators
	{
		OverloadOpStuct a13(13);
		CHECK(a13[7] == 20);
		//CHECK(&a13 == &a13.value);
		CHECK(a13->value == a13.value);
	}

	//********************************* Other operators *******************************************
	{
		OverloadOpStuct a13(13);
		CHECK(a13(1, 2) == 3);
		//CHECK((a13, 18) == (18 * 3)); //Can't overload , in D
	}
}


struct ExtOverloadOpStuct
{
	int value = 0;

	explicit ExtOverloadOpStuct(int v) : value(v) {};
};

// Unary operators
ExtOverloadOpStuct operator -(ExtOverloadOpStuct const& self) { return ExtOverloadOpStuct(-self.value); }
ExtOverloadOpStuct operator +(ExtOverloadOpStuct const& self) { return ExtOverloadOpStuct(+self.value); }
ExtOverloadOpStuct operator *(ExtOverloadOpStuct const& self) { return ExtOverloadOpStuct(self.value * self.value); }
ExtOverloadOpStuct& operator ++(ExtOverloadOpStuct& self)
{
	++self.value;
	return self;
}
ExtOverloadOpStuct& operator --(ExtOverloadOpStuct& self)
{
	--self.value;
	return self;
}
ExtOverloadOpStuct operator ++(ExtOverloadOpStuct& self, int)
{
	ExtOverloadOpStuct tmp(self);
	++self;
	return tmp;
}
ExtOverloadOpStuct operator --(ExtOverloadOpStuct& self, int)
{
	ExtOverloadOpStuct tmp(self);
	--self;
	return tmp;
}

//*************** Binary operators ************************************************************

// Arithmetic operators

ExtOverloadOpStuct& operator += (ExtOverloadOpStuct& self, ExtOverloadOpStuct const& other)
{
	self.value += other.value;
	return self;
}

ExtOverloadOpStuct operator + (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	ExtOverloadOpStuct tmp(self);
	return tmp += other;
}

ExtOverloadOpStuct& operator -= (ExtOverloadOpStuct& self, ExtOverloadOpStuct const& other)
{
	self.value -= other.value;
	return self;
}

ExtOverloadOpStuct operator - (ExtOverloadOpStuct& self, ExtOverloadOpStuct const& other)
{
	ExtOverloadOpStuct tmp(self);
	return tmp -= other;
}

ExtOverloadOpStuct& operator *= (ExtOverloadOpStuct& self, ExtOverloadOpStuct const& other)
{
	self.value *= other.value;
	return self;
}

ExtOverloadOpStuct operator * (ExtOverloadOpStuct& self, ExtOverloadOpStuct const& other)
{
	ExtOverloadOpStuct tmp(self);
	return tmp *= other;
}

ExtOverloadOpStuct& operator /= (ExtOverloadOpStuct& self, ExtOverloadOpStuct const& other)
{
	self.value /= other.value;
	return self;
}

ExtOverloadOpStuct operator / (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	ExtOverloadOpStuct tmp(self);
	return tmp /= other;
}

// relational operators

bool operator == (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	return self.value == other.value;
}

bool operator != (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	return !(self == other);
}

bool operator < (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	return (self.value < other.value);
}

bool operator > (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	return (self.value > other.value);
}

bool operator <= (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	return (self < other) || (self == other);
}

bool operator >= (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	return (self.value >= other.value);
}

// asymetric relational operators
bool operator == (ExtOverloadOpStuct const& self, OverloadOpStuct2 const& other)
{
	return self.value == other.value;
}

bool operator != (ExtOverloadOpStuct const& self, OverloadOpStuct2 const& other)
{
	return !(self == other);
}

bool operator < (ExtOverloadOpStuct const& self, OverloadOpStuct2 const& other)
{
	return (self.value < other.value);
}

bool operator > (ExtOverloadOpStuct const& self, OverloadOpStuct2 const& other)
{
	return (self.value > other.value);
}

bool operator <= (ExtOverloadOpStuct const& self, OverloadOpStuct2 const& other)
{
	return (self < other) || (self == other);
}

bool operator >= (ExtOverloadOpStuct const& self, OverloadOpStuct2 const& other)
{
	return (self.value >= other.value);
}

//************************ logical operators **************************************************
bool operator ! (ExtOverloadOpStuct const& self)
{
	return self.value == 0;
}

bool operator && (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	return (!!self) && (!!other);
}

bool operator || (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	return (!!self) || (!!other);
}

//************************ Bitwise operators **************************************************
ExtOverloadOpStuct operator ~ (ExtOverloadOpStuct const& self)
{
	return ExtOverloadOpStuct(~(self.value));
}

ExtOverloadOpStuct operator & (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	return ExtOverloadOpStuct(self.value & other.value);
}

ExtOverloadOpStuct operator | (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	return ExtOverloadOpStuct(self.value | other.value);
}

ExtOverloadOpStuct operator ^ (ExtOverloadOpStuct const& self, ExtOverloadOpStuct const& other)
{
	return ExtOverloadOpStuct(self.value ^ other.value);
}

ExtOverloadOpStuct operator << (ExtOverloadOpStuct const& self, int count)
{
	return ExtOverloadOpStuct(self.value << count);
}

ExtOverloadOpStuct operator >> (ExtOverloadOpStuct const& self, int count)
{
	return ExtOverloadOpStuct(self.value >> count);
}

// Member and pointer operators

int const* operator & (ExtOverloadOpStuct const& self)
{
	return &self.value;
}

int* operator & (ExtOverloadOpStuct& self)
{
	return &self.value;
}


void check_extern_overloaded_operator()
{
	//****************   Unary operators  *********************************************************
	{
		ExtOverloadOpStuct i(18);
		CHECK((-i).value == -18);
		CHECK((+i).value == +18);
		CHECK((~i).value == ~18);
		CHECK((*i).value == 18 * 18);
		// Pre incr and decr
		CHECK((++i).value == 19);
		CHECK(i.value == 19);
		CHECK((--i).value == 18);
		CHECK(i.value == 18);
		// post incr and decr
		CHECK((i++).value == 18);
		CHECK(i.value == 19);
		CHECK((i--).value == 19);
		CHECK(i.value == 18);
	}

	//********************* Binary operators ******************************************************
	// Arithmetic operators
	{
		ExtOverloadOpStuct a(6);
		ExtOverloadOpStuct b(3);

		ExtOverloadOpStuct mu = a * b;
		CHECK(mu.value == 18);

		ExtOverloadOpStuct d = a / b;
		CHECK(d.value == 2);

		ExtOverloadOpStuct p = a + b;
		CHECK(p.value == 9);

		ExtOverloadOpStuct mi = a - b;
		CHECK(mi.value == 3);

		mu /= b;
		CHECK(mu.value == a.value);

		d *= b;
		CHECK(d.value == a.value);

		p -= b;
		CHECK(p.value == a.value);

		mi += b;
		CHECK(mi.value == a.value);
	}

	// relational operators
	{
		ExtOverloadOpStuct a6(6);
		ExtOverloadOpStuct b6(6);
		ExtOverloadOpStuct c3(3);
		CHECK(b6 == a6);
		CHECK(b6 <= a6);
		CHECK(b6 >= a6);
		CHECK(c3 != a6);
		CHECK(c3  < a6);
		CHECK(c3 <= a6);
		CHECK(a6  > c3);
		CHECK(a6 >= c3);
	}

	// Asymetric relational operators
	{
		OverloadOpStuct2 a6 = { 6 };
		ExtOverloadOpStuct b6(6);
		ExtOverloadOpStuct c3(3);
		ExtOverloadOpStuct d9(9);
		CHECK(b6 == a6);
		CHECK(b6 <= a6);
		CHECK(b6 >= a6);
		CHECK(c3 != a6);
		CHECK(c3  < a6);
		CHECK(c3 <= a6);
		CHECK(d9  > a6);
		CHECK(d9 >= a6);
	}

	// Logical operators
	{
		ExtOverloadOpStuct a0(0);
		ExtOverloadOpStuct b0(0);
		ExtOverloadOpStuct c1(48);
		ExtOverloadOpStuct d1(79);
		CHECK(!a0);
		CHECK(!!c1);
		CHECK(!(a0 && b0));
		CHECK(!(a0 && c1));
		CHECK(!(c1 && a0));
		CHECK(c1 && d1);
		CHECK(!(a0 || b0));
		CHECK(a0 || c1);
		CHECK(c1 || a0);
		CHECK(c1 || d1);
	}

	//************************ Bitwise operators **************************************************
	{
		auto inv = ~decltype(ExtOverloadOpStuct::value)(0);
		ExtOverloadOpStuct a000(0);
		ExtOverloadOpStuct a110(6);
		ExtOverloadOpStuct a011(3);
		CHECK((~a000).value == inv);
		CHECK((a110 & a011) == ExtOverloadOpStuct(2));
		CHECK((a110 | a011) == ExtOverloadOpStuct(7));
		CHECK((a110 ^ a011) == ExtOverloadOpStuct(5));
		CHECK((a110 >> 1) == ExtOverloadOpStuct(3));
		CHECK((a110 << 1) == ExtOverloadOpStuct(12));
	}
}