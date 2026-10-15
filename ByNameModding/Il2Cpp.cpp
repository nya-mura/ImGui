#include "Includes.h"
#include "Il2Cpp.h"
#include "obfuscate.h"
#include <android/log.h>
#include <dlfcn.h>
#include "fake_dlfcn.h"
#define targetLib OBFUSCATE("libil2cpp.so")
#define g_LogTag OBFUSCATE("IL2CPP-LOGGER")

typedef unsigned short UTF16;
typedef wchar_t UTF32;
typedef char UTF8;


    const void *(*il2cpp_assembly_get_image)(const void *assembly);
    void *(*il2cpp_domain_get)();
    void **(*il2cpp_domain_get_assemblies)(const void *domain, size_t *size);
    const char *(*il2cpp_image_get_name)(void *image);
    void *(*il2cpp_class_from_name)(const void *image, const char *namespaze, const char *name);
    void *(*il2cpp_class_get_field_from_name)(void *klass, const char *name);
    void *(*il2cpp_class_get_method_from_name)(void *klass, const char *name, int argsCount);
    size_t (*il2cpp_field_get_offset)(void *field);
    void (*il2cpp_field_static_get_value)(void *field, void *value);
    void (*il2cpp_field_static_set_value)(void *field, void *value);
    void *(*il2cpp_array_new)(void *elementTypeInfo, size_t length);
    uint16_t *(*il2cpp_string_chars)(void *str);
    Il2CppString *(*il2cpp_string_new)(const char *str);
    Il2CppString *(*il2cpp_string_new_utf16)(const wchar_t *str, int32_t length);
    char *(*il2cpp_type_get_name)(void *type);
    void* (*il2cpp_method_get_param)(void *method, uint32_t index);
    void* (*il2cpp_class_get_methods)(void *klass, void* *iter);
    const char* (*il2cpp_method_get_name)(void *method);
    void *(*il2cpp_object_new)(void *klass);


void *Il2CppGetImageByName(const char *image) {
	size_t size;
	
	void **assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &size);
	
	for(int i = 0; i < size; ++i) {
		void *img = (void *)il2cpp_assembly_get_image(assemblies[i]);
		const char *img_name = il2cpp_image_get_name(img);
		
		if (strcmp(img_name, image) == 0) {
			return img;
		}
	}
	return nullptr;
}

void *Il2CppGetClassType(const char *image, const char *namespaze, const char *clazz) {
	std::map<std::string, void *> cache;
	
	std::string s = image;
	s += namespaze;
	s += clazz;
	if (cache.count(s) > 0)
		return cache[s];
		
	void *img = Il2CppGetImageByName(image);
	if (!img) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find image %s!"), image);
		return nullptr;
	}
	
	void *klass = il2cpp_class_from_name(img, namespaze, clazz);
	if (!klass) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find class %s!"), clazz);
		return nullptr;
	}
	
	cache[s] = klass;
	return klass;
}

void Il2CppGetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void *output) {
	void *img = Il2CppGetImageByName(image);
	if (!img) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find image %s!"), image);
		return;
	}
	void *klass = Il2CppGetClassType(image, namespaze, clazz);
	if (!klass) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s!"), clazz, name);
		return;
	}
    void *field = il2cpp_class_get_field_from_name(klass, name);
	if (!field) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s in class %s!"), name, clazz);
		return;
	}
	il2cpp_field_static_get_value(field, output);
}

void Il2CppSetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void* value) {
	void *img = Il2CppGetImageByName(image);
	if (!img) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find image %s!"), image);
		return;
	}
	void *klass = Il2CppGetClassType(image, namespaze, clazz);
	if (!klass) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s!"), clazz, name);
		return;
	}
	void *field = il2cpp_class_get_field_from_name(klass, name);
	if (!field) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s in class %s!"), name, clazz);
		return;
	}
	il2cpp_field_static_set_value(field, value);
}

void *Il2CppGetMethodOffset(const char *image, const char *namespaze, const char *clazz, const char *name, int argsCount) {
	void *img = Il2CppGetImageByName(image);
	if (!img) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find image %s!"), image);
		return nullptr;
	}
	void *klass = Il2CppGetClassType(image, namespaze, clazz);
	if (!klass) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find method %s!"), clazz, name);
		return nullptr;
	}
	void **method = (void**)il2cpp_class_get_method_from_name(klass, name, argsCount);
	if (!method) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find method %s in class %s!"), name, clazz);
		return nullptr;
	}
	__android_log_print(ANDROID_LOG_DEBUG, g_LogTag, OBFUSCATE("%s - [%s] %s::%s: %p"), image, namespaze, clazz, name, *method);
	return *method;
}

size_t Il2CppGetFieldOffset(const char *image, const char *namespaze, const char *clazz, const char *name) {
	void *img = Il2CppGetImageByName(image);
	if (!img) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find image %s!"), image);
		return -1;
	}
	void *klass = Il2CppGetClassType(image, namespaze, clazz);
	if (!klass) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s!"), clazz, name);
		return -1;
	}
	void *field = il2cpp_class_get_field_from_name(klass, name);
	if (!field) {
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s in class %s!"), clazz, name);
		return -1;
	}
	auto result = il2cpp_field_get_offset(field);
	__android_log_print(ANDROID_LOG_DEBUG, g_LogTag, OBFUSCATE("%s - [%s] %s::%s: %p"), image, namespaze, clazz, name, (void *) result);
	return result;
}

int is_surrogate(UTF16 uc) {
    return (uc - 0xd800u) < 2048u;
}

int is_high_surrogate(UTF16 uc) {
    return (uc & 0xfffffc00) == 0xd800;
}

int is_low_surrogate(UTF16 uc) {
    return (uc & 0xfffffc00) == 0xdc00;
}

UTF32 surrogate_to_utf32(UTF16 high, UTF16 low) {
    return (high << 10) + low - 0x35fdc00;
}

// const char* utf16_to_utf8(const UTF16* source, size_t len) {
//     std::u16string s(source, source + len);
//     std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
//     return convert.to_bytes(s).c_str();
// }


const char* utf16_to_utf8(const UTF16* source, size_t len) {
    // Convert UTF-16 (source) to UTF-8 string. Return pointer to thread-local storage.
    std::string out;
    out.reserve(len * 3 + 1);

    for (size_t i = 0; i < len; ++i) {
        uint32_t codepoint = static_cast<uint16_t>(source[i]);

        // handle surrogate pair
        if (is_high_surrogate(source[i]) && (i + 1) < len && is_low_surrogate(source[i + 1])) {
            uint16_t high = source[i];
            uint16_t low = source[++i]; // consume low surrogate
            // convert pair to codepoint
            codepoint = ((static_cast<uint32_t>(high) - 0xD800u) << 10)
                        + (static_cast<uint32_t>(low) - 0xDC00u)
                        + 0x10000u;
        }

        // encode UTF-8
        if (codepoint <= 0x7F) {
            out.push_back(static_cast<char>(codepoint));
        } else if (codepoint <= 0x7FF) {
            out.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else if (codepoint <= 0xFFFF) {
            out.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else {
            out.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        }
    }

    static thread_local std::string tls;
    tls = std::move(out);
    return tls.c_str();
}
const wchar_t* utf16_to_utf32(const UTF16* source, size_t len) {
	auto output = new UTF32[len + 1];

    for (int i = 0; i < len; i++) {
        const UTF16 uc = source[i];
        if (!is_surrogate(uc)) {
            output[i] = uc;
        }
        else {
            if (is_high_surrogate(uc) && is_low_surrogate(source[i]))
                output[i] = surrogate_to_utf32(uc, source[i]);
            else
                output[i] = L'?';
        }
    }

    output[len] = L'\0';
    return output;
}

const char* Il2CppString::CString() {
    return utf16_to_utf8(&this->start_char, this->length);
}

const wchar_t* Il2CppString::WCString() {
    return utf16_to_utf32(&this->start_char, this->length);
}

Il2CppString *Il2CppString::Create(const char *s) {
    return il2cpp_string_new(s);
}

Il2CppString *Il2CppString::Create(const wchar_t *s, int len) {
    return il2cpp_string_new_utf16(s, len);
}

void Il2CppAttach(const char *name) {
    void *handle = dlopen_ex(name, 0);
    while (!handle) {
        handle = dlopen_ex(name, 0);
        sleep(1);
    }

    il2cpp_assembly_get_image = (const void *(*)(const void *)) dlsym_ex(handle, "il2cpp_assembly_get_image");
    il2cpp_domain_get = (void *(*)()) dlsym_ex(handle, "il2cpp_domain_get");
    il2cpp_domain_get_assemblies = (void **(*)(const void* , size_t*)) dlsym_ex(handle, "il2cpp_domain_get_assemblies");
    il2cpp_image_get_name = (const char *(*)(void *)) dlsym_ex(handle, "il2cpp_image_get_name");
    il2cpp_class_from_name = (void* (*)(const void*, const char*, const char *)) dlsym_ex(handle, "il2cpp_class_from_name");
    il2cpp_class_get_field_from_name = (void* (*)(void*, const char *)) dlsym_ex(handle, "il2cpp_class_get_field_from_name");
    il2cpp_class_get_method_from_name = (void* (*)(void *, const char*, int)) dlsym_ex(handle, "il2cpp_class_get_method_from_name");
    il2cpp_field_get_offset = (size_t (*)(void *)) dlsym_ex(handle, "il2cpp_field_get_offset");
    il2cpp_field_static_get_value = (void (*)(void*, void *)) dlsym_ex(handle, "il2cpp_field_static_get_value");
    il2cpp_field_static_set_value = (void (*)(void*, void *)) dlsym_ex(handle, "il2cpp_field_static_set_value");
    il2cpp_array_new = (void *(*)(void*, size_t)) dlsym_ex(handle, "il2cpp_array_new");
    il2cpp_string_chars = (uint16_t *(*)(void*)) dlsym_ex(handle, "il2cpp_string_chars");
    il2cpp_string_new = (Il2CppString *(*)(const char *)) dlsym_ex(handle, "il2cpp_string_new");
    il2cpp_string_new_utf16 = (Il2CppString *(*)(const wchar_t *, int32_t)) dlsym_ex(handle, "il2cpp_string_new");
    il2cpp_type_get_name = (char *(*)(void *)) dlsym_ex(handle, "il2cpp_type_get_name");
    il2cpp_method_get_param = (void *(*)(void *, uint32_t)) dlsym_ex(handle, "il2cpp_method_get_param");
    il2cpp_class_get_methods = (void *(*)(void *, void **)) dlsym_ex(handle, "il2cpp_class_get_methods");
    il2cpp_method_get_name = (const char *(*)(void *)) dlsym_ex(handle, "il2cpp_method_get_name");
    il2cpp_object_new = (void *(*)(void *)) dlsym_ex(handle, "il2cpp_object_new");

    dlclose_ex(handle);
}
// void IL2Cpp::Il2CppAttach() {
// 	void *IL2Cpp_Handle = nullptr;
//
// 	while (!IL2Cpp_Handle) {
// 		IL2Cpp_Handle = dlopen(targetLib, RTLD_NOW);
// 		sleep(1);
// 	}
// 	il2cpp_domain_get_assemblies = (void **(*)(const void *, size_t *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_domain_get_assemblies"));
// 	il2cpp_string_new = (Il2CppString *(*)(const char *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_string_new"));
//     il2cpp_string_new_utf16 = (Il2CppString *(*)(const wchar_t *, int32_t)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_string_new_utf16"));
// 	il2cpp_domain_get = (void *(*)()) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_domain_get"));
//     il2cpp_assembly_get_image = (const void *(*)(const void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_assembly_get_image"));
// 	il2cpp_image_get_name = (const char *(*)(void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_image_get_name"));
//     il2cpp_class_from_name = (void* (*)(const void*, const char*, const char *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_class_from_name"));
//     il2cpp_class_get_field_from_name = (void* (*)(void*, const char *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_class_get_field_from_name"));
//     il2cpp_field_static_get_value = (void (*)(void*, void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_field_static_get_value"));
// 	il2cpp_field_static_set_value = (void (*)(void*, void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_field_static_set_value"));
//     il2cpp_class_get_method_from_name = (void* (*)(void *, const char*, int)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_class_get_method_from_name"));
//     il2cpp_field_get_offset = (size_t (*)(void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_field_get_offset"));
// 	dlclose(IL2Cpp_Handle);
// }
