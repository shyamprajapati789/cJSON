
#include "cjson.cpp"

using namespace jsonSpace;

int main()
{
  // std::string demoString_Json = "{\"i0\":5964123,\"i1\":{\"j0\":null,\"j1\":\"cvc,fgfgsdsd:\"},\"i2\":false,\"i3\":[7412,0,1,{\"k0\":null,\"k1\":\"hjggf\",\"k2\":8952},\"ewsf\",null,\"dsfdsf\",[1,2,\"ASd\",328],\"pcvkq\",4235,234,null,[true,\"dasfef\",8635]],\"i4\":[\"asdas\",345,null,\"tyhfg\",8654,[true,\"dasfef\",8635,null,{\"k0\":null,\"k1\":\"hjggf\",\"k2\":8952},null,{\"bcx\":1,\"ertg\":43,\"uyjn\":false,\"wqre\":2}],\"sd2sdf\",null,false,\"uytidf\",true,45432],\"i5\":2345}";
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

  onFreeJSONMem(tMap);
  onFreeListMem(tList);
}

