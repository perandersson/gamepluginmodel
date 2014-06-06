#pragma once
#include "pluginobject.h"

template<class T>
struct PLUGIN_API TScopedObject {
	TScopedObject() {
		mPointer = nullptr;
	}

	TScopedObject(T* ptr) {
		mPointer = ptr;
		mPointer->AddRef();
	}

	TScopedObject(const T* ptr) {
		T* cp = const_cast<T*>(ptr);
		cp->AddRef();
		mPointer = nullptr;
	}

	template<class U>
	TScopedObject(const TScopedObject<U>& o) {
		if (o.IsNotNull()) {
			mPointer = interface_cast<T>(*o);
		}
	}

	~TScopedObject() {
		if (mPointer != nullptr) {
			mPointer->Release();
			mPointer = nullptr;
		}
	}

	T* operator->() {
		return mPointer;
	}

	const T* operator->() const {
		return mPointer;
	}

	T* operator*() {
		return mPointer;
	}

	const T* operator*() const {
		return mPointer;
	}

	bool IsNull() const {
		return mPointer == nullptr;
	}

	bool IsNotNull() const {
		return mPointer != nullptr;
	}

private:
	T* mPointer;
};
