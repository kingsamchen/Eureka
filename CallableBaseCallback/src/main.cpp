
#include <functional>
#include <iostream>

#include "base/bind.h"

template<typename>
class CallableCallback;

template<typename R, typename... Args>
class CallableCallback<R(Args...)> {
public:
    using callback_t = base::Callback<R(Args...)>;

    explicit CallableCallback(callback_t callback)
        : callback_(callback)
    {}

    R operator()(Args... args) const
    {
        return callback_.Run(args...);
    }

private:
    callback_t callback_;
};

template<typename Callback>
auto MakeCallable(Callback callback)->CallableCallback<typename Callback::RunType>
{
    using Sig = typename Callback::RunType;
    return CallableCallback<Sig>(callback);
}

int Inc(int i)
{
    return ++i;
}

void Foo(const std::string&, std::string, int)
{}

class Test {
public:
    Test()
        : msg_("test"), weak_ptr_factory_(this)
    {}

    void Bark(const std::string& data)
    {
        std::cout << data << ":\t" << msg_ << std::endl;
    }

    std::string msg_;
    base::WeakPtrFactory<Test> weak_ptr_factory_;
};

int main()
{
    std::cout << "--- test case 1 ---\n";
    std::function<int(int)> fn(MakeCallable(base::Bind(&Inc)));
    std::cout << fn(5) << std::endl;

    std::cout << "--- test case 2 ---\n";
    Test* ptr = new Test();
    std::function<void()> mfn(
        MakeCallable(base::Bind(&Test::Bark, ptr->weak_ptr_factory_.GetWeakPtr(), "inside")));
    mfn();
    std::cout << "release test instance\n";
    delete ptr;
    mfn();

    return 0;
}