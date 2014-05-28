/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include <jni.h>
#include <stdexcept>

const std::string JNI_STRING_SIGNATURE("(Ljava/lang/String;)V");

/**
 * \brief Interface to Dalvik via JNI.
 */
class JNILink {
	JNIEnv 			*env; // game thread env (vs main thread env: state->activity->env)
	jobject			thiz;
	jclass			clazz;

	const android_app* refState;

public:
	JNILink(android_app* state) : refState(state) {
		if (refState->activity->vm->AttachCurrentThread(&env, nullptr) == 0) {
			thiz = refState->activity->clazz;
			clazz = env->GetObjectClass(thiz);
		}
		else {
			throw std::runtime_error("AttachCurrentThread Failed");
		}
	}

	~JNILink() {
		refState->activity->vm->DetachCurrentThread();
	}

	jmethodID GetMethodID(const std::string& name, const std::string& sig) {
		return env->GetMethodID(clazz, name.c_str(), sig.c_str());
	}

	template<typename ...Args>
	void CallVoidMethod(jmethodID& methodID, Args &&...args) {
		env->CallVoidMethod(thiz, methodID, std::forward<Args>(args)...);
	}

	jstring NewStringUTF(const std::string& str) {
		return env->NewStringUTF(str.c_str());
	}

	void DeleteLocalRef(jobject localRef) {
		env->DeleteLocalRef(localRef);
	}

	JNIEnv* GetEnv() { return env; }
	jobject& GetObject() { return thiz; }
	jclass& GetClass() { return clazz; }
};
