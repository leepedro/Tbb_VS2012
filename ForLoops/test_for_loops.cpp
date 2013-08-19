#include <vector>
#include <iostream>

//#include "tbb/tbb.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

void Print(const std::vector<int> &data)
{
	for (auto it = data.begin(), itEnd = data.end(); it != itEnd; ++it)
		std::cout << *it << ' ';
	std::cout << std::endl;
}

void Increase(int &i)
{
	i++;
}

void IncreaseSerial(std::vector<int> &data)
{
	for (auto it = data.begin(), itEnd = data.end(); it != itEnd; ++it)
		Increase(*it);
}

void IncreaseSerial(std::vector<int> &data, std::vector<int>::size_type begin,
					std::vector<int>::size_type end)
{
	for (auto it = data.begin() + begin, itEnd = data.begin() + end; it != itEnd; ++it)
		Increase(*it);
}

class IncreaseParallelFunc
{
public:
	IncreaseParallelFunc(std::vector<int> &data) : data_(data) {}

	void operator() (const tbb::blocked_range<::size_t> &r) const
	{
		for (auto it = r.begin(); it != r.end(); ++it)
			Increase(this->data_[it]);
	}

protected:
	std::vector<int> &data_;
};

void IncreaseParallel(std::vector<int> &data)
{
	tbb::parallel_for(tbb::blocked_range<::size_t>(0, data.size()), IncreaseParallelFunc(data));
}

void IncreaseParallel(std::vector<int> &data, std::vector<int>::size_type begin,
					  std::vector<int>::size_type end)
{
	tbb::parallel_for(tbb::blocked_range<::size_t>(begin, end), IncreaseParallelFunc(data));
}

int main(void)
{
	int a(0);
	Increase(a);

	std::vector<int> data(5, 0);
	Print(data);
	IncreaseSerial(data);
	Print(data);
	IncreaseParallel(data);
	Print(data);

	data.assign(4, 0);
	Print(data);
	IncreaseSerial(data, 0, 2);
	Print(data);
	IncreaseParallel(data, 2, 4);
	Print(data);
}