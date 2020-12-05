const char * CPythonBackground::GetMapName()
{
	const std::string& c_rstrMapFileName = GetWarpMapName();
	
	if (c_rstrMapFileName == "metin2_map_a1")
		return "Map1 Red";
	else if (c_rstrMapFileName == "metin2_map_b1")
		return "Map1 Yellow";
	else if (c_rstrMapFileName == "metin2_map_c1")
		return "Map1 Blue";
	//etc
	else
		return "-";
}