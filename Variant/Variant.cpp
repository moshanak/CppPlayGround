/*

variant, any, QVariant の違い

* variant
	指定した複数の型を一つの変数として保持できる

* any
	どんな型でも一つの変数として保持できる

* QVariant
	特定の複数の型を一つの変数として保持できる。
	保持できる型は決まっており、下記のURLを参照。
	https://doc.qt.io/qt-6.2/qvariant.html

* std::shared_ptr<void>
	どんな型でも一つの変数として保持できる。
	any との違いは、any は値を保持するのに対して、std::shared_ptr<void> はポインタを保持する
*/

#include <iostream>
#include <variant>
#include <any>
#include <array>

class Beam {
public:
	Beam() = default;
	explicit Beam(std::array<int, 2> connectivities) :connectivities_(connectivities) {}
	std::string getType() const { return "beam"; }
	const std::array<int, 2>& getConnectivities() const { return connectivities_; }
	//int getNumberOfNodes() const { return 2; }
private:
	std::array<int, 2> connectivities_;
};

class Shell {
public:
	Shell() = default;
	explicit Shell(std::array<int, 4> connectivities) :connectivities_(connectivities) {}
	std::string getType() const { return "shell"; }
	const std::array<int, 4>& getConnectivities() const { return connectivities_; }
	//int getNumberOfNodes() const { return 4; }
private:
	std::array<int, 4> connectivities_;
};

class Solid {
public:
	Solid() = default;
	explicit Solid(std::array<int, 8> connectivities) :connectivities_(connectivities) {}
	std::string getType() const { return "solid"; }
	const std::array<int, 8>& getConnectivities() const { return connectivities_; }
	int getNumberOfNodes() const { return 8; }
private:
	std::array<int, 8> connectivities_;
};

int main()
{
	Beam beam{ { 0, 1 } };
	Shell shell{ { 0, 1, 2, 3 } };
	Solid solid{ {0,1,2,3,4,5,6,7 } };

	// variant で指定した型を保持できるようになる
	std::variant<Beam, Shell, Solid> element;
	element = beam;
	//element = 1;	//int 型は指定していないためコンパイルエラー

	// 共通のメンバ関数であれば、visit を用いて呼ぶことができる
	std::string elementType = std::visit([](auto& e) { return e.getType(); }, element);
	std::cout << elementType << std::endl;

	// Beam と Shell では、getNumberOfNodes メンバ関数はないためコンパイルエラー
	//int numberOfNodes = std::visit([](auto& e) { return e.getNumberOfNodes(); }, element);
	// getConnectivities 関数の戻り値はバラバラなのでコンパイルエラー
	//auto connectivities = std::visit([](auto& e) { return e.getConnectivities(); }, element);

	//途中で他の型の値を代入することも可能
	element = solid;

	//現在保持している型を確認して処理を分岐できる
	if (std::holds_alternative<Beam>(element)) {
		std::cout << "is beam" << std::endl;
		Beam& beamElement = std::get<Beam>(element);
		beamElement.getConnectivities();
		beamElement.getType();
	}
	else if (std::holds_alternative<Shell>(element)) {
		std::cout << "is shell" << std::endl;
		Shell& shellElement = std::get<Shell>(element);
		shellElement.getConnectivities();
		shellElement.getType();
	}
	else if (std::holds_alternative<Solid>(element)) {
		std::cout << "is solid" << std::endl;
		Solid& solidElement = std::get<Solid>(element);
		solidElement.getConnectivities();
		solidElement.getType();
		solidElement.getNumberOfNodes();
	}

	// any は型の縛りがなく、何でも代入できる。
	std::any anyElement;
	anyElement = 1;
	anyElement = "any";
	anyElement = beam;
	// キャストしてメンバ関数を呼ぶことができる
	std::any_cast<Beam>(anyElement).getConnectivities();
	// キャストできるか不明ならば try catch を使って判定する必要がある
	try {
		std::any_cast<Shell>(anyElement).getConnectivities();
	}
	catch (...) {
		std::cout << "cast error" << std::endl;
	}

	// any と同様な機能として、shared_ptr<void> も何でも代入できる。 any との違いは、こちらはポインタになること。
	std::shared_ptr<void> spElement;
	spElement = std::make_shared<int>(1);
	spElement = std::make_shared<std::string>("any");
	spElement = std::make_shared<Beam>(std::array<int, 2> { 0, 1 });
	std::static_pointer_cast<Beam>(spElement)->getConnectivities();
}
