# ``cJSON``
## Simple and lightweight JSON and List parser in C++.

cJSON is simple and lightweight JSON and Python stype List parser. Given a JSON compliant String (std::string), the code returns a C++ Map (std::map) object. No extra libraries required. Just copy and paste cjson.cpp in your repo/code directory, include the file in your code and call the function onEvaluate(yourString).

## Features

- Can handle Strings, NULL/None, Numbers Lists, Sub-Jsons and Sub-Lists.
- Can parse Python style Lists when given as a string.
- Easy to setup and use.
- Tested with Flask jsonify() results.

## Usage

```c++

#include "cjson.cpp"

using namespace jsonSpace;

int main()
{
  std::string demoString_Json = "{\"k0\":5964123,\"k1\":{\"j0\":null,\"j1\":\"someString\"},\"k2\":false,\"k3\":[7412,435.25,56.695]}";

  auto rVals = onEvaluate(demoString_Json);
  std::map<std::string, jsonValue*> tMap = std::get<0>(rVals);
  std::cout << "\n Data : " << demoString_Json << std::endl;
  onPrintJSON(tMap);


  std::string demoString_List = "[89,210,564,{\"k0\":null,\"k1\":\"some str0\",\"k2\":8952},\"some str1\",null,\"some str2\",[1,2,\"ASd\",328],\"more string\",4235,234,null,[true,\"even more string\",8635]]";

  rVals = onEvaluate(demoString_List, false);
  jsonValue* tList = std::get<1>(rVals);
  std::cout << "\n Data : " << demoString_List << std::endl;
  onPrintList(tList);
}

```
# Output
```sh
Data : {"k0":5964123,"k1":{"j0":null,"j1":"someString"},"k2":false,"k3":[7412,435.25,56.695]}
  {
    k0 : 5964123.000000,
      {
        j0 : NULL,
        j1 : "someString",
      }
    k2 : false,
    k3 :
      [
        7412.000000,
        435.250000,
        56.695000,
      ],
  }

 Data : [89,210,564,{"k0":null,"k1":"some str0","k2":8952},"some str1",null,"some str2",[1,2,"ASd",328],"more string",4235,234,null,[true,"even more string",8635]]
  [
    89.000000,
    210.000000,
    564.000000,
      {
        k0 : NULL,
        k1 : "some str0",
        k2 : 8952.000000,
      }
    "some str1",
    NULL,
    "some str2",
      [
        1.000000,
        2.000000,
        "ASd",
        328.000000,
      ],
    "more string",
    4235.000000,
    234.000000,
    NULL,
      [
        true,
        "even more string",
        8635.000000,
      ],
  ],
```

> Note : All keys and string values "within" the std::string variable must be enclosed in double quotes("). For example "{"someKey" : "someValue"}". When declaring such a string in C++, the quotes must be backed by a "backslash" (\\). Hence, the example becomes "{\\"someKey\\" : \\"someValue\\"}". For Lists a valid example is "[234, NULL, \\"someString\\", false]"

# onEvalute(...) Function.
The ```onEvaluate(...)``` function is what does the hardwork.
```c++
// onEvaluate() defination.
std::tuple< std::map<std::string, jsonValue*>, jsonValue*, size_t> onEvaluate(std::string const &tData, bool isJson=true, std::size_t stIdx=0, std::size_t enIdx=0, bool doVerbose=false );

/*
The function takes the following parameters as input.
    1 : std::string tData -> The String to evaluate ofcourse.
    2 : bool isJson -> Set true if you need to evaluate a json, ot false otherwise.
        Default -> true.
    3 : std::size_t stIdx -> Index position where evaluation must start from.
        Default -> 0
    4 : std::size_t enIdx=0 -> Index position where evaluation must end.
        Default -> 0 (Which internally translates to length of the input string tData)
    5 : bool doVerbose -> Enable prints to Debug.
        Default -> false

The function returns a tuple of 3 values. The reason for this is to use a single function to evaluate JSON or Lists. Also this helps a bit on recursion.
The first (or zeroth if that is how you want to count) value of the tuple is a std::map object. When evaluating a JSON this is the return you are intrested in. The second (or first...) element is a pointer to a jsonValue object. Evaluate a list, and this is the return you are looking for. The third element is used for recursion while evaluating and can be ignored.
*/

```
