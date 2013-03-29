#include "pb2json.h"
#include <boost/format.hpp>

using std::string;

string pb2json(const Message *msg)
{
        if (!msg) {
            return NULL;
        }
        Json::Value root;
	bool result = parse_msg(*msg, root);
        if (result) {
            return root.toStyledString();	    
        } else {
            return NULL;
        }	
}

string pb2json(Message *msg, const char *buf, int len)
{
        if (!msg) {
            return NULL;
        }
	string s (buf,len);
	msg->ParseFromString(s);
	return pb2json(msg); 	
}

static bool parse_repeated_field(const Message& msg, const Reflection& ref, const FieldDescriptor* field, Json::Value& node)
{
	size_t count = ref.FieldSize(msg, field);
	switch(field->cpp_type())
	{
		case FieldDescriptor::CPPTYPE_DOUBLE:
			for(size_t i = 0 ; i != count ; ++i)
			{
				double	value1 = ref.GetRepeatedDouble(msg, field,i);
				node.append(Json::Value(value1));
			}
			break;
		case FieldDescriptor::CPPTYPE_FLOAT:
			for(size_t i = 0 ; i != count ; ++i)
			{
				float value2 = ref.GetRepeatedFloat(msg, field,i);
				node.append(Json::Value(value2));
			}
			break;
		case FieldDescriptor::CPPTYPE_INT64:
			for(size_t i = 0 ; i != count ; ++i)
			{
				int64_t value3 = ref.GetRepeatedInt64(msg, field,i);
				node.append(Json::Value((boost::format("%d") % value3).str()));
			}
			break;
		case FieldDescriptor::CPPTYPE_UINT64:
			for(size_t i = 0 ; i != count ; ++i)
			{
				uint64_t value4 = ref.GetRepeatedUInt64(msg, field,i);
				node.append(Json::Value((boost::format("%d") % value4).str()));
			}
			break;
		case FieldDescriptor::CPPTYPE_INT32:
			for(size_t i = 0 ; i != count ; ++i)
			{
				int32_t value5 = ref.GetRepeatedInt32(msg, field,i);
				node.append(Json::Value(value5));
			}
			break;
		case FieldDescriptor::CPPTYPE_UINT32:
			for(size_t i = 0 ; i != count ; ++i)
			{
				uint32_t value6 = ref.GetRepeatedUInt32(msg, field,i);
				node.append(Json::Value(value6));
			}
			break;
		case FieldDescriptor::CPPTYPE_BOOL:
			for(size_t i = 0 ; i != count ; ++i)
			{
				bool value7 = ref.GetRepeatedBool(msg, field,i);
				node.append(Json::Value(value7))	;
			}
			break;
		case FieldDescriptor::CPPTYPE_STRING:
			for(size_t i = 0 ; i != count ; ++i)
			{
				string value8 = ref.GetRepeatedString(msg, field,i);
				node.append(Json::Value(value8.c_str()));
			}
			break;
		case FieldDescriptor::CPPTYPE_MESSAGE:
			for(size_t i = 0 ; i != count ; ++i)
			{
				const Message *value9 = &(ref.GetRepeatedMessage(msg,field,i));
				Json::Value repeatedNode;
				parse_msg(*value9, repeatedNode);
				node.append(repeatedNode);
			}
			break;
		case FieldDescriptor::CPPTYPE_ENUM:
			for(size_t i = 0 ; i != count ; ++i)
			{
				const EnumValueDescriptor* value10 = ref.GetRepeatedEnum(msg, field,i);
				node.append(Json::Value(value10->number()));
			}
			break;
		default:
			break;
	}
	return true;
}


static bool parse_msg(const Message& msg, Json::Value& root)
{
	const Descriptor *d = msg.GetDescriptor();
	if(!d)return false;
	size_t count = d->field_count();

	for (size_t i = 0; i != count ; ++i)
	{
		const FieldDescriptor *field = d->field(i);
		if(!field) return false;

		const Reflection *ref = msg.GetReflection();
		if(!ref)return false;
		const string name = field->name().c_str();

		if(field->is_repeated()) {
			Json::Value node;			
			parse_repeated_field(msg, *ref, field, node);
			root[name] = node;
			continue;
		}

		if(ref->HasField(msg,field))
		{
			switch (field->cpp_type())
			{
				case FieldDescriptor::CPPTYPE_DOUBLE:
				{
					double value1 = ref->GetDouble(msg,field);
					root[name] = Json::Value(value1);
					break;
				}
				case FieldDescriptor::CPPTYPE_FLOAT:
				{
					float value2 = ref->GetFloat(msg,field);
					root[name] = Json::Value(value2);
					break;
				}
				case FieldDescriptor::CPPTYPE_INT64:
				{
					int64_t value3 = ref->GetInt64(msg,field);
					root[name] = Json::Value((boost::format("%d") % value3).str());
					break;
				}
				case FieldDescriptor::CPPTYPE_UINT64:
				{
					uint64_t value4 = ref->GetUInt64(msg,field);
					root[name] = Json::Value((boost::format("%d") % value4).str());
					break;
				}
				case FieldDescriptor::CPPTYPE_INT32:
				{
					int32_t value5 = ref->GetInt32(msg,field);
					root[name] = Json::Value(value5);
					break;
				}
				case FieldDescriptor::CPPTYPE_UINT32:
				{
					uint32_t value6 = ref->GetUInt32(msg,field);
					root[name] = Json::Value(value6);
					break;
				}
				case FieldDescriptor::CPPTYPE_BOOL:
				{
					bool value7 = ref->GetBool(msg,field);
					root[name] = Json::Value(value7);
					break;
				}
				case FieldDescriptor::CPPTYPE_STRING:
				{
					string value8 = ref->GetString(msg,field);
					if (value8.length() > 0) {
						root[name] = Json::Value(value8.c_str());
					}
					break;
				}
				case FieldDescriptor::CPPTYPE_MESSAGE:
				{
					const Message *value9 = &(ref->GetMessage(msg,field));
					Json::Value subNode;
					parse_msg(*value9, subNode);
					root[name] = subNode;
					break;
				}
				case FieldDescriptor::CPPTYPE_ENUM:
				{
					const EnumValueDescriptor *value10 = ref->GetEnum(msg,field);
					root[name] = Json::Value(value10->number());
					break;
				}
				default:
					break;
			}

		}

	}
	return true;
}
