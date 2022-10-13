/*

範囲 for に関する変更

・概要
	* 初期化式をともなう範囲for文

・詳細（★は下部にサンプルコードあり）
	★10.1 初期化式をともなう範囲for文
	https://cpprefjp.github.io/lang/cpp20/range-based_for_statements_with_initializer.html
	10.2 範囲for文がカスタマイゼーションポイントを見つけるルールを緩和
	https://cpprefjp.github.io/lang/cpp20/relaxing_the_range_for_loop_customization_point_finding_rules.html

・所感
	10.1 は、たまに遭遇するケース。
*/

#include <iostream>
#include <vector>
#include <random>
#include <numeric>

struct Point {
	float x, y, z;
};

class Model {
public:
	Model(const std::vector<Point>& coordinates) :coordinates_(coordinates) {}
	const std::vector<Point>& getCoordinates() const { return coordinates_; }
private:
	std::vector<Point> coordinates_;
};

Model getModel() {
	std::vector<Point> coordinates;
	std::random_device seed_gen;
	std::default_random_engine engine(seed_gen());
	std::uniform_real_distribution<> dist(100.0f, -100.0f);
	for (int i = 0; i < 2; ++i) {
		float x = static_cast<float>(dist(engine));
		float y = static_cast<float>(dist(engine));
		float z = static_cast<float>(dist(engine));
		coordinates.emplace_back(x, y, z);
	}
	return Model(coordinates);
}

int main()
{
	Model model = getModel();

	{
		// C++17 までは nodeIndex をスコープ外に出さないためには {} で区切る必要があった
		int nodeIndex = 0;
		for (const Point& node : model.getCoordinates()) {
			std::cout << "nodeIndex = " << nodeIndex << "\tx = " << node.x << "\ty = " << node.y << "\tz = " << node.z << std::endl;
			++nodeIndex;
		}
	}
	// あるいは、範囲 for文 で書かない
	for (int nodeIndex = 0; nodeIndex < static_cast<int>(model.getCoordinates().size()); ++nodeIndex) {
		const Point& node = model.getCoordinates().at(nodeIndex);
		std::cout << "nodeIndex = " << nodeIndex << "\tx = " << node.x << "\ty = " << node.y << "\tz = " << node.z << std::endl;
	}
	// C++20 からは範囲 for 文でも、最初に初期化式を記述できる
	for (int nodeIndex = 0; const Point & node : model.getCoordinates()) {
		std::cout << "nodeIndex = " << nodeIndex << "\tx = " << node.x << "\ty = " << node.y << "\tz = " << node.z << std::endl;
		++nodeIndex;
	}

	// 以降は余談。
	
	for (const Point & node : getModel().getCoordinates()) {
		// 未定義動作で、visual studio で試すと通らない。getModel().getCoordinates() 実行時は Model クラスは生存しているが、それ以降は破棄されているため。
		std::cout << "Not executed." << std::endl;
		std::cout << "x = " << node.x << "\ty = " << node.y << "\tz = " << node.z << std::endl;
	}

	for (Model model = getModel(); const Point & node : model.getCoordinates()) {
		// 通る。初期化式により、 for 文内では Model クラスオブジェクトが生存するようになる。
		std::cout << "Executed." << std::endl;
		std::cout << "x = " << node.x << "\ty = " << node.y << "\tz = " << node.z << std::endl;
	}

	// 通常の for 文と同様に、同じ型なら複数の初期化が可能
	for (int nodeIndex = 0, nodeID = 1; const Point & node : model.getCoordinates()) {
		std::cout << "nodeIndex = " << nodeIndex << "\tnodeID = " << nodeID << "\tx = " << node.x << "\ty = " << node.y << "\tz = " << node.z << std::endl;
		++nodeIndex;
		++nodeID;
	}

	// 初期化式で構造化束縛は NG。コンパイルエラーになる。
#if 0
	for (auto [nodeIndex, model2] = std::pair<int, Model>(0, getModel()); const Point & node : model2.getCoordinates()) {
		std::cout << "nodeIndex = " << nodeIndex << "\tx = " << node.x << "\ty = " << node.y << "\tz = " << node.z << std::endl;
		++nodeIndex;
	}
#endif

	return 0;
}