#pragma once

#include <type_traits>
#include <concepts>
#include <utility>
#include <typeindex>
#include <iostream>

template <class T>
concept NotAny = !std::same_as<std::remove_cvref_t<T>, class Any>;

class Any;

class StorageBase {
public:
    StorageBase(std::type_index index) : index_(index) {
    }
    virtual ~StorageBase() = default;

    std::type_index Info() const {
        return index_;
    }

    virtual StorageBase* CreateStorage() = 0;

private:
    std::type_index index_;
};

template <typename T>
class Storage : public StorageBase {
public:
    Storage(T value) : StorageBase(typeid(T)), value_(std::forward<T>(value)) {
    }
    T& Get() {
        return value_;
    }
    ~Storage() = default;

    StorageBase* CreateStorage() override {
        return new Storage<T>(value_);
    }

private:
    T value_;
};

class Any {
public:
    Any() {
    }

    // T&& - universal (forwarding) reference
    // use std::forward inside this constructor
    template <NotAny T>
    Any(T&& value) {
        storage_ptr_ = new Storage<std::decay_t<T>>(std::forward<T>(value));
    }

    Any(const Any& other) {
        if (other.storage_ptr_ == nullptr) {
            storage_ptr_ = nullptr;
        } else {
            storage_ptr_ = other.storage_ptr_->CreateStorage();
        }
    }

    Any(Any&& other) {
        storage_ptr_ = other.storage_ptr_;
        other.storage_ptr_ = nullptr;
    }

    Any& operator=(const Any& other) {
        if (storage_ptr_ != other.storage_ptr_) {
            StorageBase* tmp_ptr = nullptr;

            if (other.storage_ptr_ == nullptr) {
                tmp_ptr = nullptr;
            } else {
                tmp_ptr = other.storage_ptr_->CreateStorage();
            }

            Clear();
            storage_ptr_ = tmp_ptr;
        }
        return *this;
    }

    Any& operator=(Any&& other) {
        Clear();
        storage_ptr_ = other.storage_ptr_;
        other.storage_ptr_ = nullptr;
        return *this;
    }

    bool operator==(const Any& other) const {
        return storage_ptr_ == other.storage_ptr_;
    }

    ~Any() {
        Clear();
    }

    bool Empty() const {
        return storage_ptr_ == nullptr;
    }

    void Clear() {
        delete storage_ptr_;
        storage_ptr_ = nullptr;
    }

    void Swap(Any& other) {
        std::swap(this->storage_ptr_, other.storage_ptr_);
    }

    template <class T>
    const T& GetValue() const {
        if (Empty() || storage_ptr_->Info() != typeid(T)) {
            throw std::bad_cast();
        }

        return static_cast<Storage<T>*>(storage_ptr_)->Get();
    }

private:
    StorageBase* storage_ptr_ = nullptr;
};
