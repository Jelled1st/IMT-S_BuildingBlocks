// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Country.generated.h"

UENUM(BlueprintType)
enum Country
{
	Afghanistan UMETA(DisplayName = "Afghanistan"),
	Albania UMETA(DisplayName = "Albania"),
	Algeria UMETA(DisplayName = "Algeria"),
	Andorra UMETA(DisplayName = "Andorra"),
	Angola UMETA(DisplayName = "Angola"),
	Antigua_and_Barbuda UMETA(DisplayName = "Antigua_and_Barbuda"),
	Argentina UMETA(DisplayName = "Argentina"),
	Armenia UMETA(DisplayName = "Armenia"),
	Australia UMETA(DisplayName = "Australia"),
	Austria UMETA(DisplayName = "Austria"),
	Azerbaijan UMETA(DisplayName = "Azerbaijan"),
	The_Bahamas UMETA(DisplayName = "The_Bahamas"),
	Bahrain UMETA(DisplayName = "Bahrain"),
	Bangladesh UMETA(DisplayName = "Bangladesh"),
	Barbados UMETA(DisplayName = "Barbados"),
	Belarus UMETA(DisplayName = "Belarus"),
	Belgium UMETA(DisplayName = "Belgium"),
	Belize UMETA(DisplayName = "Belize"),
	Benin UMETA(DisplayName = "Benin"),
	Bhutan UMETA(DisplayName = "Bhutan"),
	Bolivia UMETA(DisplayName = "Bolivia"),
	Bosnia_and_Herzegovina UMETA(DisplayName = "Bosnia_and_Herzegovina"),
	Botswana UMETA(DisplayName = "Botswana"),
	Brazil UMETA(DisplayName = "Brazil"),
	Brunei UMETA(DisplayName = "Brunei"),
	Bulgaria UMETA(DisplayName = "Bulgaria"),
	Burkina_Faso UMETA(DisplayName = "Burkina_Faso"),
	Burundi UMETA(DisplayName = "Burundi"),
	Cambodia UMETA(DisplayName = "Cambodia"),
	Cameroon UMETA(DisplayName = "Cameroon"),
	Canada UMETA(DisplayName = "Canada"),
	Cape_Verde UMETA(DisplayName = "Cape_Verde"),
	Central_African_Republic UMETA(DisplayName = "Central_African_Republic"),
	Chad UMETA(DisplayName = "Chad"),
	Chile UMETA(DisplayName = "Chile"),
	China UMETA(DisplayName = "China"),
	Colombia UMETA(DisplayName = "Colombia"),
	Comoros UMETA(DisplayName = "Comoros"),
	Congo_Republic_of_the UMETA(DisplayName = "Congo_Republic_of_the"),
	Congo_Democratic_Republic_of_the UMETA(DisplayName = "Congo"),
	Costa_Rica UMETA(DisplayName = "Costa_Rica"),
	Cote_dIvoire UMETA(DisplayName = "Cote_dIvoire"),
	Croatia UMETA(DisplayName = "Croatia"),
	Cuba UMETA(DisplayName = "Cuba"),
	Cyprus UMETA(DisplayName = "Cyprus"),
	Czech_Republic UMETA(DisplayName = "Czech_Republic"),
	Denmark UMETA(DisplayName = "Denmark"),
	Djibouti UMETA(DisplayName = "Djibouti"),
	Dominica UMETA(DisplayName = "Dominica"),
	Dominican_Republic UMETA(DisplayName = "Dominican_Republic"),
	East_Timor UMETA(DisplayName = "East_Timor"),
	Ecuador UMETA(DisplayName = "Ecuador"),
	Egypt UMETA(DisplayName = "Egypt"),
	El_Salvador UMETA(DisplayName = "El_Salvador"),
	Equatorial_Guinea UMETA(DisplayName = "Equatorial_Guinea"),
	Eritrea UMETA(DisplayName = "Eritrea"),
	Estonia UMETA(DisplayName = "Estonia"),
	Ethiopia UMETA(DisplayName = "Ethiopia"),
	Fiji UMETA(DisplayName = "Fiji"),
	Finland UMETA(DisplayName = "Finland"),
	France UMETA(DisplayName = "France"),
	Gabon UMETA(DisplayName = "Gabon"),
	The_Gambia UMETA(DisplayName = "The_Gambia"),
	Georgia UMETA(DisplayName = "Georgia"),
	Germany UMETA(DisplayName = "Germany"),
	Ghana UMETA(DisplayName = "Ghana"),
	Greece UMETA(DisplayName = "Greece"),
	Grenada UMETA(DisplayName = "Grenada"),
	Guatemala UMETA(DisplayName = "Guatemala"),
	Guinea UMETA(DisplayName = "Guinea"),
	Guinea_Bissau UMETA(DisplayName = "Guinea_Bissau"),
	Guyana UMETA(DisplayName = "Guyana"),
	Haiti UMETA(DisplayName = "Haiti"),
	Honduras UMETA(DisplayName = "Honduras"),
	Hungary UMETA(DisplayName = "Hungary"),
	Iceland UMETA(DisplayName = "Iceland"),
	India UMETA(DisplayName = "India"),
	Indonesia UMETA(DisplayName = "Indonesia"),
	Iran UMETA(DisplayName = "Iran"),
	Iraq UMETA(DisplayName = "Iraq"),
	Ireland UMETA(DisplayName = "Ireland"),
	Israel UMETA(DisplayName = "Israel"),
	Italy UMETA(DisplayName = "Italy"),
	Jamaica UMETA(DisplayName = "Jamaica"),
	Japan UMETA(DisplayName = "Japan"),
	Jordan UMETA(DisplayName = "Jordan"),
	Kazakhstan UMETA(DisplayName = "Kazakhstan"),
	Kenya UMETA(DisplayName = "Kenya"),
	Kiribati UMETA(DisplayName = "Kiribati"),
	Korea_North UMETA(DisplayName = "Korea_North"),
	Korea_South UMETA(DisplayName = "Korea_South"),
	Kosovo UMETA(DisplayName = "Kosovo"),
	Kuwait UMETA(DisplayName = "Kuwait"),
	Kyrgyzstan UMETA(DisplayName = "Kyrgyzstan"),
	Laos UMETA(DisplayName = "Laos"),
	Latvia UMETA(DisplayName = "Latvia"),
	Lebanon UMETA(DisplayName = "Lebanon"),
	Lesotho UMETA(DisplayName = "Lesotho"),
	Liberia UMETA(DisplayName = "Liberia"),
	Libya UMETA(DisplayName = "Libya"),
	Liechtenstein UMETA(DisplayName = "Liechtenstein"),
	Lithuania UMETA(DisplayName = "Lithuania"),
	Luxembourg UMETA(DisplayName = "Luxembourg"),
	Macedonia UMETA(DisplayName = "Macedonia"),
	Madagascar UMETA(DisplayName = "Madagascar"),
	Malawi UMETA(DisplayName = "Malawi"),
	Malaysia UMETA(DisplayName = "Malaysia"),
	Maldives UMETA(DisplayName = "Maldives"),
	Mali UMETA(DisplayName = "Mali"),
	Malta UMETA(DisplayName = "Malta"),
	Marshall_Islands UMETA(DisplayName = "Marshall_Islands"),
	Mauritania UMETA(DisplayName = "Mauritania"),
	Mauritius UMETA(DisplayName = "Mauritius"),
	Mexico UMETA(DisplayName = "Mexico"),
	Micronesia_Federated_States_of UMETA(DisplayName = "Micronesia_Federated_States_of"),
	Moldova UMETA(DisplayName = "Moldova"),
	Monaco UMETA(DisplayName = "Monaco"),
	Mongolia UMETA(DisplayName = "Mongolia"),
	Montenegro UMETA(DisplayName = "Montenegro"),
	Morocco UMETA(DisplayName = "Morocco"),
	Mozambique UMETA(DisplayName = "Mozambique"),
	Myanmar_Burma UMETA(DisplayName = "Myanmar_Burma"),
	Namibia UMETA(DisplayName = "Namibia"),
	Nauru UMETA(DisplayName = "Nauru"),
	Nepal UMETA(DisplayName = "Nepal"),
	Netherlands UMETA(DisplayName = "Netherlands"),
	New_Zealand UMETA(DisplayName = "New_Zealand"),
	Nicaragua UMETA(DisplayName = "Nicaragua"),
	Niger UMETA(DisplayName = "Niger"),
	Nigeria UMETA(DisplayName = "Nigeria"),
	Norway UMETA(DisplayName = "Norway"),
	Oman UMETA(DisplayName = "Oman"),
	Pakistan UMETA(DisplayName = "Pakistan"),
	Palau UMETA(DisplayName = "Palau"),
	Panama UMETA(DisplayName = "Panama"),
	Papua_New_Guinea UMETA(DisplayName = "Papua_New_Guinea"),
	Paraguay UMETA(DisplayName = "Paraguay"),
	Peru UMETA(DisplayName = "Peru"),
	Philippines UMETA(DisplayName = "Philippines"),
	Poland UMETA(DisplayName = "Poland"),
	Portugal UMETA(DisplayName = "Portugal"),
	Qatar UMETA(DisplayName = "Qatar"),
	Romania UMETA(DisplayName = "Romania"),
	Russia UMETA(DisplayName = "Russia"),
	Rwanda UMETA(DisplayName = "Rwanda"),
	Saint_Kitts_and_Nevis UMETA(DisplayName = "Saint_Kitts_and_Nevis"),
	Saint_Lucia UMETA(DisplayName = "Saint_Lucia"),
	Saint_Vincent_and_the_Grenadines UMETA(DisplayName = "Saint_Vincent_and_the_Grenadines"),
	Samoa UMETA(DisplayName = "Samoa"),
	San_Marino UMETA(DisplayName = "San_Marino"),
	Sao_Tome_and_Principe UMETA(DisplayName = "Sao_Tome_and_Principe"),
	Saudi_Arabia UMETA(DisplayName = "Saudi_Arabia"),
	Senegal UMETA(DisplayName = "Senegal"),
	Serbia UMETA(DisplayName = "Serbia"),
	Seychelles UMETA(DisplayName = "Seychelles"),
	Sierra_Leone UMETA(DisplayName = "Sierra_Leone"),
	Singapore UMETA(DisplayName = "Singapore"),
	Slovakia UMETA(DisplayName = "Slovakia"),
	Slovenia UMETA(DisplayName = "Slovenia"),
	Solomon_Islands UMETA(DisplayName = "Solomon_Islands"),
	Somalia UMETA(DisplayName = "Somalia"),
	South_Africa UMETA(DisplayName = "South_Africa"),
	South_Sudan UMETA(DisplayName = "South_Sudan"),
	Spain UMETA(DisplayName = "Spain"),
	Sri_Lanka UMETA(DisplayName = "Sri_Lanka"),
	Sudan UMETA(DisplayName = "Sudan"),
	Suriname UMETA(DisplayName = "Suriname"),
	Swaziland UMETA(DisplayName = "Swaziland"),
	Sweden UMETA(DisplayName = "Sweden"),
	Switzerland UMETA(DisplayName = "Switzerland"),
	Syria UMETA(DisplayName = "Syria"),
	Taiwan UMETA(DisplayName = "Taiwan"),
	Tajikistan UMETA(DisplayName = "Tajikistan"),
	Tanzania UMETA(DisplayName = "Tanzania"),
	Thailand UMETA(DisplayName = "Thailand"),
	Togo UMETA(DisplayName = "Togo"),
	Tonga UMETA(DisplayName = "Tonga"),
	Trinidad_and_Tobago UMETA(DisplayName = "Trinidad_and_Tobago"),
	Tunisia UMETA(DisplayName = "Tunisia"),
	Turkey UMETA(DisplayName = "Turkey"),
	Turkmenistan UMETA(DisplayName = "Turkmenistan"),
	Tuvalu UMETA(DisplayName = "Tuvalu"),
	Uganda UMETA(DisplayName = "Uganda"),
	Ukraine UMETA(DisplayName = "Ukraine"),
	United_Arab_Emirates UMETA(DisplayName = "United_Arab_Emirates"),
	United_Kingdom UMETA(DisplayName = "United_Kingdom"),
	United_States_of_America UMETA(DisplayName = "United_States_of_America"),
	Uruguay UMETA(DisplayName = "Uruguay"),
	Uzbekistan UMETA(DisplayName = "Uzbekistan"),
	Vanuatu UMETA(DisplayName = "Vanuatu"),
	Vatican_City_Holy_See UMETA(DisplayName = "Vatican_City_Holy_See"),
	Venezuela UMETA(DisplayName = "Venezuela"),
	Vietnam UMETA(DisplayName = "Vietnam"),
	Yemen UMETA(DisplayName = "Yemen"),
	Zambia UMETA(DisplayName = "Zambia"),
	Zimbabwe UMETA(DisplayName = "Zimbabwe"),
};
