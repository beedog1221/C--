#ifndef ENTITYBOX_TYPEINFO_H
#define ENTITYBOX_TYPEINFO_H
#include <typeinfo>
#include <cassert>
namespace eb { 
		// Shamelessly copied from Loki.
	class TypeInfo {
		public:
			// Constructors
			TypeInfo(); // needed for containers
			TypeInfo(const std::type_info&); // non-explicit
	
			// Access for the wrapped std::type_info
			const std::type_info& Get() const;
			// Compatibility functions
			bool before(const TypeInfo& rhs) const;
			const char* name() const;
	
		private:
			const std::type_info* pInfo_;
	};
		
	// Implementation
		
	inline TypeInfo::TypeInfo() {
		class Nil {};
		pInfo_ = &typeid(Nil);
		assert(pInfo_);
	}
	
	inline TypeInfo::TypeInfo(const std::type_info& ti)
	: pInfo_(&ti)
	{ assert(pInfo_); }
	
	inline bool TypeInfo::before(const TypeInfo& rhs) const {
		assert(pInfo_);
		// type_info::before return type is int in some VC libraries 
		return pInfo_->before(*rhs.pInfo_) != 0;
	}

	inline const std::type_info& TypeInfo::Get() const {
		assert(pInfo_);
		return *pInfo_;
	}
	
	inline const char* TypeInfo::name() const {
		assert(pInfo_);
		return pInfo_->name();
	}

// Comparison operators
	
	inline bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)

	// type_info::operator== return type is int in some VC libraries
	{ return (lhs.Get() == rhs.Get()) != 0; }

	inline bool operator<(const TypeInfo& lhs, const TypeInfo& rhs)
	{ return lhs.before(rhs); }

	inline bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs)
	{ return !(lhs == rhs); }    
	
	inline bool operator>(const TypeInfo& lhs, const TypeInfo& rhs)
	{ return rhs < lhs; }
	
	inline bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs)
	{ return !(lhs > rhs); }
	 
	inline bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs)
	{ return !(lhs < rhs); }
}
#endif