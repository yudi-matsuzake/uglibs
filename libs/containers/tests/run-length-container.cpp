#include "catch2/catch.hpp"

#include "containers/run-length-container.hpp"
#include "util/misc-adaptors.hpp"

#include <stdexcept>

void semantic_equal(
		util::run_length_container<char> const& rlc,
		std::vector<char> const& gt)
{
	REQUIRE(rlc.size() == gt.size());
	for(auto i=0UL; i<gt.size(); ++i){
		INFO("testing pos = " << i);
		REQUIRE(gt[i] == rlc.at(i));
	}
}

template<class F>
void vector_comparison(
		util::run_length_container<char>& rlc,
		std::vector<char>& gt,
		F&& f)
{
	f(rlc);
	f(gt);
	semantic_equal(rlc, gt);
}

template<class F>
void run_length_check(
		util::run_length_container<char>& rlc,
		std::vector<char>& gt,
		std::vector<util::run_length_item<char>> const& rl_gt,
		F&& f)
{
	f(rlc);
	f(gt);
	semantic_equal(rlc, gt);

	REQUIRE(rlc.base().size() == rl_gt.size());
	for(auto i=0UL; auto const& c : rlc.base()){
		INFO("testing rl pos: " << i);
		REQUIRE(c == rl_gt[i]);
		i++;
	}
}

TEST_CASE("sorting", "[run-length-container]")
{
	constexpr auto N = 30;
	util::run_length_container<char> rlc;
	std::vector<char> gt;

	// push back 
	vector_comparison(rlc, gt,
			[c = util::make_random_sparse_char_array<N>('a', 'd', 5)]
			(auto& v)
			{
				for(auto&& i : c)
					v.push_back(i);
			}
	);

	// sorting
	vector_comparison(rlc, gt,
			[](auto& v)
			{
				std::sort(begin(v), end(v));
			}
	);

}

TEST_CASE("simple operations", "[run-length-container]")
{
	util::run_length_container<char> rlc;
	std::vector<char> gt;

	// push back 
	run_length_check(rlc, gt,
			{ { 'a', 3 }, { 'b', 7 }, { 'c', 8 } },
			[](auto& v)
			{
				v.push_back('a');
				v.push_back('a');
				v.push_back('a');
				v.push_back('b');
				v.push_back('b');
				v.push_back('b');
				v.push_back('b');
				v.push_back('c');
			}
	);

	// set testing: beginning case
	run_length_check(rlc, gt,
			{ { 'a', 4 }, { 'b', 7 }, { 'c', 8 } },
			[](auto& v)
			{
				v.at(3) = 'a';
			}
	);

	// setting all indexes
	run_length_check(rlc, gt,
			{ { 'a', 4 }, { 'b', 7 }, { 'c', 8 } },
			[](auto& v)
			{
				v.at(0) = 'a';
				v.at(1) = 'a';
				v.at(2) = 'a';
				v.at(3) = 'a';
				v.at(4) = 'b';
				v.at(5) = 'b';
				v.at(6) = 'b';
				v.at(7) = 'c';
			}
	);

	// set testing: ending case
	run_length_check(rlc, gt,
			{ { 'a', 4 }, { 'b', 6 }, { 'c', 8 } },
			[](auto& v)
			{
				v.at(6) = 'c';
			}
	);

	// set testing: need to remove case
	run_length_check(rlc, gt,
			{ { 'a', 5 }, { 'c', 8 } },
			[](auto& v)
			{
				v.at(5) = 'c';
				v.at(4) = 'a';
			}
	);

	// set testing: include new rl item
	run_length_check(rlc, gt,
			{ { 'a', 4 }, { 'd', 5 }, { 'c', 8 } },
			[](auto& v)
			{
				v.at(4) = 'd';
			}
	);

	// set testing: setting the value of a rl item with count = 1
	run_length_check(rlc, gt,
			{ { 'a', 4 }, { 'b', 5 }, { 'c', 8 } },
			[](auto& v)
			{
				v.at(4) = 'b';
			}
	);

	run_length_check(rlc, gt,
			{ { 'a', 4 }, { 'b', 5 }, { 'a', 6 }, { 'c', 8 } },
			[](auto& v)
			{
				v.at(5) = 'a';
			}
	);

	// set testing: deleting and mergin
	run_length_check(rlc, gt,
			{ { 'a', 6 }, { 'c', 8 } },
			[](auto& v)
			{
				v.at(4) = 'a';
			}
	);

	// set testing: splitting
	run_length_check(rlc, gt,
			{ { 'a', 1 }, { 'b', 2 }, { 'a', 6 }, { 'c', 8 } },
			[](auto& v)
			{
				v.at(1) = 'b';
			}
	);

	// resize test growing
	run_length_check(rlc, gt,
			{ { 'a', 1 }, { 'b', 2 }, { 'a', 6 }, { 'c', 8 }, { 'd', 10 } },
			[](auto& v)
			{
				v.resize(10);
				v.at(8) = 'd';
				v.at(9) = 'd';
			}
	);

	// resize test shrinking
	run_length_check(rlc, gt,
			{ { 'a', 1 }, { 'b', 2 }, { 'a', 5 } },
			[](auto& v)
			{
				v.resize(5);
			}
	);
}


TEST_CASE("iterators", "[run-length-container]")
{

	constexpr auto N = 50;
	util::run_length_container<char> rlc;
	std::vector<char> gt;

	// push back 
	vector_comparison(rlc, gt,
			[c = util::make_random_sparse_char_array<N>('a', 'd', 5)](auto& v)
			{
				for(auto&& i : c)
					v.push_back(i);
			}
	);

	REQUIRE(std::equal(begin(rlc), end(rlc), begin(gt)));
	REQUIRE(std::equal(rbegin(rlc), rend(rlc), rbegin(gt)));

	auto set_flag = util::generate_array<bool, N>(
		[]{ return static_cast<bool>(std::rand()%2); });

	for(auto i=0; auto&& rli : rlc){
		INFO("testing pos " << i);
		REQUIRE(rli == gt[i]);
		if(set_flag[i]){
			auto c = 'a' + rand()%('f' - 'a');
			rli = c;
			gt[i] = c;
		}

		REQUIRE(rli == gt[i]);
		i++;
	}

	REQUIRE(std::equal(begin(rlc), end(rlc), begin(gt)));
	REQUIRE(std::equal(rbegin(rlc), rend(rlc), rbegin(gt)));

}

TEST_CASE("random tests", "[run-length-container]")
{
	util::run_length_container<char> rlc;
	std::vector<char> gt;

	constexpr auto N = 100;

	vector_comparison(rlc, gt, [
			&,
			a = util::make_random_sparse_char_array<N>('a', 'c', 5)
	](auto&& v)
	{
		for(auto i=0u; i<a.size(); ++i)
			v.push_back(a[i]);
	});

	vector_comparison(rlc, gt, [
		&,
		ai = util::make_random_indices<N>(rlc.size()),
		ac = util::make_random_sparse_char_array<N>('a', 'c', 5)
	](auto&& v)
	{
		for(auto i=0u; i<ai.size(); ++i)
			v.at(ai[i]) = ac[i];
	});

}
