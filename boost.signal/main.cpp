// Beginner hello world example for Boost.Signals2
// Copyright Douglas Gregor 2001-2004.
// Copyright Frank Mori Hess 2009.
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// For more information, see http://www.boost.org

#include <iostream>
#include <boost/signals2/signal.hpp>

//[ hello_world_def_code_snippet
struct HelloWorld
{
	float operator()() const
	{
		std::cout << "Hello, World! 111" << std::endl;

		return 1;
	}
};
//]

struct HelloWorld2
{
	float operator()() const
	{
		std::cout << "Hello, World! 222" << std::endl;

		return 2;
	}
};

struct HelloWorld3
{
	float operator()() const
	{
		std::cout << "Hello, World! 333" << std::endl;

		return 3;
	}
};

// combiner which returns the maximum value returned by all slots
template<typename T>
struct maximum
{
	typedef T result_type;

	template<typename InputIterator>
	T operator()(InputIterator first, InputIterator last) const
	{
		// If there are no slots to call, just return the
		// default-constructed value
		//if (first == last) return T();
		T max_value;// = *first++;
		int haha = 2;
		int cur = 0;
		while (first != last) {
			std::cout << "aa" << std::endl;
			//if (cur == haha)
			//	return 1;
			//++cur;

			++first;
		}

		return max_value;
	}
};

int main()
{
	//[ hello_world_single_code_snippet
	// Signal with no arguments and a void return value
	boost::signals2::signal<float(),  maximum<float>> sig;

	// Connect a HelloWorld slot
	HelloWorld3 hello3;
	sig.connect(hello3);
	HelloWorld hello;
	sig.connect(hello);
	HelloWorld2 hello2;
	sig.connect(hello2);


	// Call all of the slots
	std::cout<<sig()<<std::endl;
	//]

	return 0;
}



