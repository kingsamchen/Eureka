/*
 @ 0xCCCCCCCC
*/

#ifndef RATE_LIMIT_BUCKET_LIMITER_H_
#define RATE_LIMIT_BUCKET_LIMITER_H_

class Bucket {
public:
    ~Bucket() = default;

    Bucket(const Bucket&) = delete;

    Bucket(Bucket&&) = delete;

    Bucket& operator=(const Bucket&) = delete;

    Bucket& operator=(Bucket&&) = delete;

private:

};

#endif  // RATE_LIMIT_BUCKET_LIMITER_H_
