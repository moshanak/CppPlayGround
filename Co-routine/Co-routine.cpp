/*

コルーチン

・概要：
　関数の途中で中断や再開ができる

・メリット
　非同期処理を実装しやすくなる。（自身で別スレッドを生成する必要がない。）

・所感
　下記の理由で、現状は使う必要はない。
　　* C++20の時点では低レベルのAPIしか提供されておらず、メリットの実装しやすさがない。（下記のサンプルコード参照。）
  　　より使いやすいライブラリの提供は、C++23以降に予定されている。
　　* 別スレッドを生成すれば、コルーチンと同じような処理を実装できる。
*/


#include <iostream>
#include <coroutine>
#include <utility>
#include <thread>
#include <chrono>


// generator クラスは cpprefjp より引用した。
// generator クラスのようなものは、 C++23 以降に C++ 標準ライブラリが用意する予定。（generator 以外にも、非同期I/O、非同期タスクも予定しているらしい。）
// generator クラスを見ると、C++20 時点では実装しやすいメリットがないことが分かる。

// コルーチン利用ライブラリ: ジェネレータ型
struct generator {
	// ジェネレータに関連付けられるPromise型
	struct promise_type {
		// co_yield式で指定されるint値を保持する変数
		int value_;

		auto get_return_object()
		{
			// コルーチンに紐づくPromiseオブジェクト(*this)から
			// ジェネレータ型のコルーチン戻り値オブジェクトを生成
			return generator{ *this };
		};
		auto initial_suspend()
		{
			// コルーチン本体処理の開始前に無条件サスペンド
			return std::suspend_always{};
		}
		auto final_suspend() noexcept
		{
			// コルーチン本体処理の終了後に無条件サスペンド
			return std::suspend_always{};
		}
		auto yield_value(int v)
		{
			// co_yield式で渡される値を保持し、コルーチンを無条件サスペンド
			value_ = v;
			return std::suspend_always{};
		}
		void return_void() {}
		void unhandled_exception() { std::terminate(); }
	};
	// ジェネレータに関連付けられるコルーチンハンドル型
	using coro_handle = std::coroutine_handle<promise_type>;

	// 範囲for構文サポート用イテレータ型
	struct iterator {
		// 対象のコルーチンハンドル
		coro_handle coro_;
		// 対象コルーチン本体処理が終了したかを表すフラグ
		bool done_;

		iterator& operator++()
		{
			// yield_value()で中断したコルーチンを再開する
			coro_.resume();
			// (co_yield式評価もしくはコルーチン本体処理の終了により制御が戻ってくる)
			done_ = coro_.done();
			return *this;
		}
		bool operator!=(const iterator& rhs) const
		{
			return done_ != rhs.done_;
		}
		int operator*() const
		{
			// Promiseオブジェクトが保持している値を返す
			return coro_.promise().value_;
		}
	};

	~generator()
	{
		if (coro_)
			coro_.destroy();
	}

	generator(generator const&) = delete;
	generator(generator&& rhs)
		: coro_(std::exchange(rhs.coro_, nullptr)) {}

	// 範囲for構文サポート用のメンバ関数
	iterator begin()
	{
		// initial_suspend()で中断したコルーチンを再開する
		coro_.resume();
		// (初回co_yield式評価により制御が戻ってくる)
		return { coro_, coro_.done() };
	}
	iterator end()
	{
		// 終端位置を表現する番兵イテレータ
		return { {}, true };
	}

private:
	// Promiseオブジェクト経由でコルーチンハンドルを取得する
	explicit generator(promise_type& p)
		: coro_(coro_handle::from_promise(p)) {}

	coro_handle coro_;
};

// ユーザ定義コルーチン
generator progress()
{
	co_yield 0;

	std::cout << "... wait process 1 ..." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));

	co_yield 10;

	std::cout << "... wait process 2 ..." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));

	co_yield 20;

	int progressRate = 20;
	while (progressRate < 100) {
		std::cout << "... wait process 3 ..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		progressRate += 10;
		co_yield progressRate;
	}
}

int main()
{
	// コルーチンを呼び出し
	auto p = progress();
	// このタイミングではまだコルーチン本体は実行されない。

	// 範囲for構文を用いてコルーチン本体を実行する。
	// main() と progress() が交互に実行されることを確認できる。
	for (int v : p) {
		//プログレスバーの表示と更新
		std::cout << "progress" << v << "%" << std::endl;
	}
}