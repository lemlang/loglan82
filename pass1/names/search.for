      subroutine init(hash)
      implicit integer*2 (a-z)
      dimension hash(8000)
      HASH (    59 ) =  1038
      HASH (    60 ) = -2343
      HASH (    81 ) =  2058
      HASH (    82 ) = -2691
      HASH (    85 ) =  1051
      HASH (    86 ) = -2271
      HASH (    95 ) =  2065
      HASH (    96 ) = -2689
      HASH (    97 ) =  2066
      HASH (    98 ) = -2639
      HASH (   115 ) =  2075
      HASH (   116 ) = -2681
      HASH (   179 ) =  1098
      HASH (   180 ) = -2669
      HASH (   189 ) =  1103
      HASH (   190 ) = -2285
      HASH (   195 ) =  1106
      HASH (   196 ) = -2865
      HASH (   209 ) =  1113
      HASH (   210 ) = -2289
      HASH (   237 ) =  2136
      HASH (   238 ) = -2485
      HASH (   307 ) =  1162
      HASH (   308 ) = -2607
      HASH (   317 ) =  1167
      HASH (   331 ) =  1174
      HASH (   332 ) = -2523
      HASH (   333 ) =  1175
      HASH (   334 ) =  2861
      HASH (   335 ) =  1176
      HASH (   336 ) = -2609
      HASH (   343 ) =  1180
      HASH (   344 ) =  2605
      HASH (   579 ) =  1298
      HASH (   580 ) = -2835
      HASH (   691 ) =  1354
      HASH (   692 ) = -2637
      HASH (   717 ) =  1367
      HASH (   719 ) =  1368
      HASH (   720 ) = -2833
      HASH (   819 ) =  1418
      HASH (   820 ) = -2831
      HASH (   827 ) =  1422
      HASH (   828 ) = -2327
      HASH (   835 ) =  1426
      HASH (   836 ) = -2395
      HASH (   847 ) =  1432
      HASH (   848 ) = -2829
      HASH (   955 ) =  1486
      HASH (   956 ) = -2827
      HASH (   975 ) =  1496
      HASH (   976 ) = -2825
      HASH (   987 ) =  1502
      HASH (   988 ) = -1989
      HASH (  1081 ) =  1549
      HASH (  1105 ) =  1561
      HASH (  1106 ) = -2363
      HASH (  1109 ) =  1563
      HASH (  1110 ) =  2819
      HASH (  1113 ) =  1565
      HASH (  1114 ) = -2815
      HASH (  1115 ) =  1566
      HASH (  1116 ) = -2807
      HASH (  1203 ) =  1610
      HASH (  1204 ) = -2493
      HASH (  1231 ) =  1624
      HASH (  1232 ) = -2241
      HASH (  1237 ) =  1627
      HASH (  1238 ) = -2797
      HASH (  1243 ) =  1630
      HASH (  1244 ) = -2341
      HASH (  1303 ) =   651
      HASH (  1304 ) = -2615
      HASH (  1305 ) =   652
      HASH (  1306 ) = -2305
      HASH (  1325 ) =   662
      HASH (  1326 ) = -2381
      HASH (  1327 ) =   663
      HASH (  1328 ) = -2999
      HASH (  1335 ) =   667
      HASH (  1336 ) = -2997
      HASH (  1339 ) =   669
      HASH (  1340 ) = -2985
      HASH (  1371 ) =  1694
      HASH (  1372 ) = -2777
      HASH (  1437 ) =   718
      HASH (  1438 ) = -2973
      HASH (  1451 ) =   725
      HASH (  1452 ) = -2969
      HASH (  1457 ) =   728
      HASH (  1458 ) = -2965
      HASH (  1459 ) =  1738
      HASH (  1460 ) = -2659
      HASH (  1463 ) =   731
      HASH (  1464 ) = -2267
      HASH (  1467 ) =  1742
      HASH (  1468 ) = -2775
      HASH (  1487 ) =  1752
      HASH (  1488 ) = -2583
      HASH (  1557 ) =   778
      HASH (  1558 ) = -2955
      HASH (  1571 ) =   785
      HASH (  1572 ) = -2949
      HASH (  1573 ) =   786
      HASH (  1574 ) = -2103
      HASH (  1579 ) =   789
      HASH (  1580 ) = -2941
      HASH (  1585 ) =   792
      HASH (  1586 ) = -2927
      HASH (  1591 ) =  1804
      HASH (  1592 ) = -2189
      HASH (  1595 ) =  1806
      HASH (  1596 ) = -2751
      HASH (  1603 ) =  1810
      HASH (  1604 ) = -2663
      HASH (  1609 ) =  1813
      HASH (  1610 ) = -2463
      HASH (  1619 ) =  1818
      HASH (  1620 ) = -2591
      HASH (  1625 ) =  1821
      HASH (  1626 ) = -2743
      HASH (  1635 ) =  1826
      HASH (  1636 ) = -2003
      HASH (  1685 ) =   842
      HASH (  1686 ) = -2349
      HASH (  1693 ) =   846
      HASH (  1694 ) = -2909
      HASH (  1701 ) =   850
      HASH (  1702 ) = -2905
      HASH (  1713 ) =   856
      HASH (  1714 ) =  2899
      HASH (  1715 ) =  1866
      HASH (  1716 ) = -2731
      HASH (  1719 ) =   859
      HASH (  1720 ) = -2283
      HASH (  1723 ) =  1870
      HASH (  1724 ) = -2647
      HASH (  1729 ) =  1873
      HASH (  1730 ) = -2727
      HASH (  1731 ) =  1874
      HASH (  1732 ) = -2593
      HASH (  1743 ) =  1880
      HASH (  1749 ) =  1883
      HASH (  1750 ) = -2721
      HASH (  1751 ) =  1884
      HASH (  1752 ) =  2425
      HASH (  1763 ) =  1890
      HASH (  1764 ) = -2719
      HASH (  1829 ) =  1367
      HASH (  1830 ) =  2253
      HASH (  1831 ) =   653
      HASH (  1832 ) = -1829
      HASH (  1833 ) =  1742
      HASH (  1834 ) = -1831
      HASH (  1835 ) =   917
      HASH (  1836 ) = -2893
      HASH (  1837 ) =   918
      HASH (  1838 ) = -2399
      HASH (  1839 ) =   919
      HASH (  1840 ) = -2891
      HASH (  1841 ) =   920
      HASH (  1842 ) = -2617
      HASH (  1849 ) =   924
      HASH (  1850 ) = -2889
      HASH (  1859 ) =   929
      HASH (  1860 ) = -2887
      HASH (  1869 ) =  1943
      HASH (  1870 ) = -2717
      HASH (  1873 ) =  1945
      HASH (  1874 ) = -2709
      HASH (  1941 ) =   970
      HASH (  1942 ) = -2877
      HASH (  1957 ) =   978
      HASH (  1958 ) =  2339
      HASH (  1969 ) =   984
      HASH (  1970 ) = -2873
      HASH (  1971 ) =  1994
      HASH (  1972 ) = -2699
      HASH (  1981 ) =   990
      HASH (  1982 ) = -2871
      HASH (  1987 ) =  2002
      HASH (  1988 ) = -2697
      HASH (  1989 ) =  1422
      HASH (  1990 ) = -1991
      HASH (  1991 ) =  1755
      HASH (  1992 ) = -1993
      HASH (  1993 ) =  1563
      HASH (  1995 ) =  1890
      HASH (  1996 ) = -1997
      HASH (  1997 ) =  1614
      HASH (  1998 ) = -1999
      HASH (  1999 ) =  1755
      HASH (  2000 ) = -2001
      HASH (  2001 ) =  1563
      HASH (  2003 ) =  1806
      HASH (  2004 ) = -2005
      HASH (  2005 ) =  1755
      HASH (  2006 ) = -2007
      HASH (  2007 ) =  1563
      HASH (  2017 ) =  1559
      HASH (  2019 ) =  1874
      HASH (  2020 ) = -2017
      HASH (  2021 ) =  1810
      HASH (  2022 ) = -2019
      HASH (  2023 ) =  1624
      HASH (  2024 ) = -2021
      HASH (  2025 ) =  1741
      HASH (  2027 ) =   778
      HASH (  2028 ) = -2025
      HASH (  2029 ) =  1496
      HASH (  2030 ) = -2027
      HASH (  2031 ) =  1181
      HASH (  2033 ) =  1694
      HASH (  2034 ) = -2031
      HASH (  2035 ) =    14
      HASH (  2037 ) =   725
      HASH (  2038 ) = -2035
      HASH (  2039 ) =  1802
      HASH (  2040 ) = -2037
      HASH (  2041 ) =   850
      HASH (  2042 ) = -2039
      HASH (  2043 ) =  1358
      HASH (  2045 ) =   651
      HASH (  2046 ) = -2043
      HASH (  2047 ) =   919
      HASH (  2048 ) = -2045
      HASH (  2049 ) =  1629
      HASH (  2051 ) =   782
      HASH (  2052 ) = -2049
      HASH (  2053 ) =   652
      HASH (  2054 ) = -2051
      HASH (  2055 ) =  2254
      HASH (  2057 ) =  1810
      HASH (  2058 ) = -2055
      HASH (  2059 ) =   661
      HASH (  2060 ) = -2057
      HASH (  2061 ) =  1742
      HASH (  2062 ) = -2059
      HASH (  2063 ) =    14
      HASH (  2065 ) =  1187
      HASH (  2066 ) = -2063
      HASH (  2067 ) =  1884
      HASH (  2068 ) = -2065
      HASH (  2069 ) =  1175
      HASH (  2070 ) = -2067
      HASH (  2071 ) =   908
      HASH (  2073 ) =  1883
      HASH (  2074 ) = -2071
      HASH (  2075 ) =  1630
      HASH (  2076 ) = -2073
      HASH (  2077 ) =   908
      HASH (  2079 ) =  1883
      HASH (  2080 ) = -2077
      HASH (  2081 ) =  1038
      HASH (  2082 ) = -2079
      HASH (  2083 ) =  1492
      HASH (  2085 ) =  1362
      HASH (  2086 ) = -2083
      HASH (  2087 ) =  1943
      HASH (  2088 ) = -2085
      HASH (  2089 ) =   916
      HASH (  2091 ) =  1806
      HASH (  2092 ) = -2089
      HASH (  2093 ) =   797
      HASH (  2094 ) =  2041
      HASH (  2095 ) =  1742
      HASH (  2096 ) = -2093
      HASH (  2097 ) =   850
      HASH (  2098 ) = -2095
      HASH (  2099 ) =   908
      HASH (  2101 ) =   929
      HASH (  2102 ) = -2099
      HASH (  2103 ) =  1739
      HASH (  2105 ) =    16
      HASH (  2107 ) =  1175
      HASH (  2108 ) = -2105
      HASH (  2109 ) =  1883
      HASH (  2110 ) = -2107
      HASH (  2111 ) =  1884
      HASH (  2112 ) = -2109
      HASH (  2113 ) =  1566
      HASH (  2114 ) = -2111
      HASH (  2115 ) =  1864
      HASH (  2117 ) =  1559
      HASH (  2118 ) = -2115
      HASH (  2119 ) =  1103
      HASH (  2120 ) = -2117
      HASH (  2121 ) =    29
      HASH (  2122 ) =  2119
      HASH (  2123 ) =  1559
      HASH (  2124 ) = -2121
      HASH (  2125 ) =  1103
      HASH (  2126 ) = -2123
      HASH (  2127 ) =    20
      HASH (  2129 ) =   652
      HASH (  2130 ) = -2127
      HASH (  2131 ) =  1883
      HASH (  2132 ) = -2129
      HASH (  2133 ) =   665
      HASH (  2135 ) =  1750
      HASH (  2136 ) = -2133
      HASH (  2137 ) =  2136
      HASH (  2138 ) = -2135
      HASH (  2139 ) =    25
      HASH (  2141 ) =  1418
      HASH (  2142 ) = -2139
      HASH (  2143 ) =  1563
      HASH (  2144 ) = -2141
      HASH (  2145 ) =   665
      HASH (  2146 ) =  2075
      HASH (  2147 ) =  1878
      HASH (  2148 ) = -2145
      HASH (  2149 ) =  1630
      HASH (  2150 ) = -2147
      HASH (  2151 ) =   665
      HASH (  2152 ) =  2081
      HASH (  2153 ) =  1878
      HASH (  2154 ) = -2151
      HASH (  2155 ) =  1038
      HASH (  2156 ) = -2153
      HASH (  2157 ) =    33
      HASH (  2158 ) =  2069
      HASH (  2159 ) =  1618
      HASH (  2160 ) = -2157
      HASH (  2161 ) =  1175
      HASH (  2162 ) = -2159
      HASH (  2163 ) =  1564
      HASH (  2164 ) =  2161
      HASH (  2165 ) =  2201
      HASH (  2166 ) = -2163
      HASH (  2167 ) =  1175
      HASH (  2168 ) = -2165
      HASH (  2169 ) =  1564
      HASH (  2170 ) =  2167
      HASH (  2171 ) =  2137
      HASH (  2172 ) = -2169
      HASH (  2173 ) =  1175
      HASH (  2174 ) = -2171
      HASH (  2175 ) =    34
      HASH (  2176 ) =  2023
      HASH (  2177 ) =  1633
      HASH (  2178 ) = -2175
      HASH (  2179 ) =  1624
      HASH (  2180 ) = -2177
      HASH (  2181 ) =  2146
      HASH (  2182 ) =  2149
      HASH (  2183 ) =  1809
      HASH (  2184 ) = -2181
      HASH (  2185 ) =  1630
      HASH (  2186 ) = -2183
      HASH (  2187 ) =   919
      HASH (  2189 ) =  1742
      HASH (  2190 ) = -2187
      HASH (  2191 ) =    24
      HASH (  2193 ) =   846
      HASH (  2194 ) = -2191
      HASH (  2195 ) =  2002
      HASH (  2196 ) = -2193
      HASH (  2197 ) =   923
      HASH (  2199 ) =  1741
      HASH (  2200 ) = -2197
      HASH (  2201 ) =   728
      HASH (  2202 ) = -2199
      HASH (  2203 ) =   925
      HASH (  2205 ) =  1365
      HASH (  2206 ) = -2203
      HASH (  2207 ) =  1610
      HASH (  2208 ) = -2205
      HASH (  2209 ) =  1500
      HASH (  2210 ) =  2173
      HASH (  2211 ) =  1870
      HASH (  2212 ) = -2209
      HASH (  2213 ) =  1175
      HASH (  2214 ) = -2211
      HASH (  2215 ) =  1751
      HASH (  2216 ) =  2207
      HASH (  2217 ) =  1870
      HASH (  2218 ) = -2215
      HASH (  2219 ) =  1610
      HASH (  2220 ) = -2217
      HASH (  2221 ) =    14
      HASH (  2223 ) =  2197
      HASH (  2224 ) = -2221
      HASH (  2225 ) =  1821
      HASH (  2226 ) = -2223
      HASH (  2227 ) =    27
      HASH (  2229 ) =  1368
      HASH (  2230 ) = -2227
      HASH (  2231 ) =   792
      HASH (  2232 ) = -2229
      HASH (  2233 ) =    21
      HASH (  2235 ) =  1173
      HASH (  2236 ) = -2233
      HASH (  2237 ) =  1999
      HASH (  2238 ) = -2235
      HASH (  2239 ) =    29
      HASH (  2240 ) =  2179
      HASH (  2241 ) =  1175
      HASH (  2242 ) = -2239
      HASH (  2243 ) =   788
      HASH (  2244 ) =  2087
      HASH (  2245 ) =  1610
      HASH (  2246 ) = -2243
      HASH (  2247 ) =  1943
      HASH (  2248 ) = -2245
      HASH (  2249 ) =  1870
      HASH (  2250 ) = -2251
      HASH (  2251 ) =  2141
      HASH (  2253 ) =  1742
      HASH (  2254 ) = -2255
      HASH (  2255 ) =  1806
      HASH (  2256 ) = -2257
      HASH (  2257 ) =    29
      HASH (  2258 ) =  2259
      HASH (  2259 ) =  1742
      HASH (  2260 ) = -2261
      HASH (  2261 ) =  2075
      HASH (  2262 ) = -2263
      HASH (  2263 ) =  1181
      HASH (  2264 ) = -2265
      HASH (  2265 ) =    14
      HASH (  2266 ) =  2061
      HASH (  2267 ) =   906
      HASH (  2268 ) = -2269
      HASH (  2269 ) =    20
      HASH (  2271 ) =  1559
      HASH (  2272 ) =  2273
      HASH (  2273 ) =  1051
      HASH (  2274 ) = -2275
      HASH (  2275 ) =  1551
      HASH (  2276 ) = -2277
      HASH (  2277 ) =    15
      HASH (  2279 ) =  1432
      HASH (  2280 ) = -2281
      HASH (  2281 ) =  1998
      HASH (  2283 ) =   672
      HASH (  2285 ) =  1173
      HASH (  2286 ) = -2287
      HASH (  2287 ) =    21
      HASH (  2288 ) =  2125
      HASH (  2289 ) =   656
      HASH (  2290 ) = -2291
      HASH (  2291 ) =    14
      HASH (  2293 ) =  1098
      HASH (  2294 ) = -2295
      HASH (  2295 ) =  1804
      HASH (  2296 ) = -2297
      HASH (  2297 ) =  1170
      HASH (  2299 ) =  1175
      HASH (  2300 ) = -2301
      HASH (  2301 ) =  1294
      HASH (  2302 ) = -2303
      HASH (  2303 ) =    34
      HASH (  2304 ) =  2213
      HASH (  2305 ) =   782
      HASH (  2306 ) = -2307
      HASH (  2307 ) =  1755
      HASH (  2308 ) = -2309
      HASH (  2309 ) =  1563
      HASH (  2310 ) =  2053
      HASH (  2311 ) =   792
      HASH (  2312 ) = -2313
      HASH (  2313 ) =  1486
      HASH (  2314 ) = -2315
      HASH (  2315 ) =  1755
      HASH (  2316 ) = -2317
      HASH (  2317 ) =  1563
      HASH (  2318 ) =  2231
      HASH (  2319 ) =  1368
      HASH (  2320 ) = -2321
      HASH (  2321 ) =  1038
      HASH (  2322 ) = -2323
      HASH (  2323 ) =  1755
      HASH (  2324 ) = -2325
      HASH (  2325 ) =  1563
      HASH (  2327 ) =  1422
      HASH (  2328 ) = -2329
      HASH (  2329 ) =  1755
      HASH (  2330 ) = -2331
      HASH (  2331 ) =  1563
      HASH (  2333 ) =    28
      HASH (  2335 ) =   789
      HASH (  2336 ) = -2333
      HASH (  2337 ) =  1358
      HASH (  2339 ) =   978
      HASH (  2340 ) = -2337
      HASH (  2341 ) =    29
      HASH (  2342 ) =  2185
      HASH (  2343 ) =    29
      HASH (  2344 ) =  2155
      HASH (  2345 ) =   919
      HASH (  2347 ) =  1561
      HASH (  2348 ) = -2345
      HASH (  2349 ) =  1870
      HASH (  2351 ) =    27
      HASH (  2352 ) =  2101
      HASH (  2353 ) =  1610
      HASH (  2354 ) = -2351
      HASH (  2355 ) =   908
      HASH (  2356 ) = -2353
      HASH (  2357 ) =   929
      HASH (  2358 ) = -2355
      HASH (  2359 ) =    28
      HASH (  2360 ) =  2347
      HASH (  2361 ) =  1559
      HASH (  2362 ) = -2359
      HASH (  2363 ) =  1874
      HASH (  2364 ) = -2361
      HASH (  2365 ) =    20
      HASH (  2366 ) =  2335
      HASH (  2367 ) =  1548
      HASH (  2368 ) = -2365
      HASH (  2369 ) =   789
      HASH (  2370 ) = -2367
      HASH (  2371 ) =   925
      HASH (  2373 ) =  1500
      HASH (  2374 ) = -2371
      HASH (  2375 ) =  1738
      HASH (  2376 ) = -2373
      HASH (  2377 ) =    27
      HASH (  2379 ) =   718
      HASH (  2380 ) = -2377
      HASH (  2381 ) =   918
      HASH (  2382 ) = -2379
      HASH (  2383 ) =  1756
      HASH (  2385 ) =  1102
      HASH (  2386 ) = -2383
      HASH (  2387 ) =  1565
      HASH (  2388 ) = -2385
      HASH (  2389 ) =  1870
      HASH (  2391 ) =  1358
      HASH (  2392 ) = -2389
      HASH (  2393 ) =   846
      HASH (  2394 ) = -2391
      HASH (  2395 ) =    23
      HASH (  2397 ) =    34
      HASH (  2399 ) =  1629
      HASH (  2400 ) = -2397
      HASH (  2401 ) =  1757
      HASH (  2402 ) =  2299
      HASH (  2403 ) =  1806
      HASH (  2404 ) = -2401
      HASH (  2405 ) =  1175
      HASH (  2406 ) = -2403
      HASH (  2407 ) =    25
      HASH (  2408 ) =  2247
      HASH (  2409 ) =   788
      HASH (  2410 ) = -2407
      HASH (  2411 ) =  1368
      HASH (  2412 ) = -2409
      HASH (  2413 ) =  1943
      HASH (  2414 ) = -2411
      HASH (  2415 ) =    25
      HASH (  2416 ) =  2319
      HASH (  2417 ) =   788
      HASH (  2418 ) = -2415
      HASH (  2419 ) =  1368
      HASH (  2420 ) = -2417
      HASH (  2421 ) =  1945
      HASH (  2423 ) =    25
      HASH (  2425 ) =  1884
      HASH (  2426 ) = -2423
      HASH (  2427 ) =    23
      HASH (  2429 ) =  1181
      HASH (  2430 ) = -2427
      HASH (  2431 ) =  2058
      HASH (  2432 ) = -2429
      HASH (  2433 ) =   667
      HASH (  2434 ) =  2225
      HASH (  2435 ) =  1561
      HASH (  2436 ) = -2433
      HASH (  2437 ) =  1821
      HASH (  2438 ) = -2435
      HASH (  2439 ) =    25
      HASH (  2440 ) =  2431
      HASH (  2441 ) =  1181
      HASH (  2442 ) = -2439
      HASH (  2443 ) =  2058
      HASH (  2444 ) = -2441
      HASH (  2445 ) =    25
      HASH (  2446 ) =  2437
      HASH (  2447 ) =  1561
      HASH (  2448 ) = -2445
      HASH (  2449 ) =  1821
      HASH (  2450 ) = -2447
      HASH (  2451 ) =    25
      HASH (  2452 ) =  1833
      HASH (  2453 ) =  1422
      HASH (  2454 ) = -2451
      HASH (  2455 ) =  1822
      HASH (  2456 ) = -2453
      HASH (  2457 ) =  1742
      HASH (  2458 ) = -2455
      HASH (  2459 ) =    20
      HASH (  2461 ) =  1490
      HASH (  2462 ) = -2459
      HASH (  2463 ) =  1568
      HASH (  2464 ) = -2461
      HASH (  2465 ) =   924
      HASH (  2467 ) =  1548
      HASH (  2468 ) = -2465
      HASH (  2469 ) =  1627
      HASH (  2470 ) = -2467
      HASH (  2471 ) =    27
      HASH (  2472 ) =  2091
      HASH (  2473 ) =   984
      HASH (  2474 ) = -2471
      HASH (  2475 ) =  1418
      HASH (  2476 ) = -2473
      HASH (  2477 ) =  1806
      HASH (  2478 ) = -2475
      HASH (  2479 ) =  1943
      HASH (  2480 ) =  2047
      HASH (  2481 ) =   859
      HASH (  2482 ) = -2479
      HASH (  2483 ) =   919
      HASH (  2484 ) = -2481
      HASH (  2485 ) =    27
      HASH (  2486 ) =  2137
      HASH (  2487 ) =    28
      HASH (  2488 ) =  2219
      HASH (  2489 ) =   930
      HASH (  2490 ) = -2487
      HASH (  2491 ) =  1364
      HASH (  2492 ) = -2489
      HASH (  2493 ) =  1486
      HASH (  2494 ) = -2491
      HASH (  2495 ) =  1167
      HASH (  2497 ) =   878
      HASH (  2498 ) = -2495
      HASH (  2499 ) =   663
      HASH (  2500 ) = -2497
      HASH (  2501 ) =    15
      HASH (  2502 ) =  2499
      HASH (  2503 ) =   850
      HASH (  2504 ) = -2501
      HASH (  2505 ) =   663
      HASH (  2506 ) = -2503
      HASH (  2507 ) =     3
      HASH (  2509 ) =   673
      HASH (  2510 ) = -2507
      HASH (  2511 ) =  1174
      HASH (  2512 ) = -2509
      HASH (  2513 ) =   673
      HASH (  2514 ) =  2511
      HASH (  2515 ) =  1174
      HASH (  2516 ) = -2513
      HASH (  2517 ) =     3
      HASH (  2518 ) =  2515
      HASH (  2519 ) =  1175
      HASH (  2520 ) = -2517
      HASH (  2521 ) =  1174
      HASH (  2522 ) = -2519
      HASH (  2523 ) =  1175
      HASH (  2524 ) =  2521
      HASH (  2525 ) =   669
      HASH (  2526 ) =  2457
      HASH (  2527 ) =  1614
      HASH (  2528 ) = -2525
      HASH (  2529 ) =  1742
      HASH (  2530 ) = -2527
      HASH (  2531 ) =  1485
      HASH (  2532 ) =  2529
      HASH (  2533 ) =  2066
      HASH (  2534 ) = -2531
      HASH (  2535 ) =  1742
      HASH (  2536 ) = -2533
      HASH (  2537 ) =    14
      HASH (  2538 ) =  2483
      HASH (  2539 ) =  1173
      HASH (  2540 ) = -2537
      HASH (  2541 ) =   847
      HASH (  2542 ) = -2539
      HASH (  2543 ) =   919
      HASH (  2544 ) = -2541
      HASH (  2545 ) =   663
      HASH (  2547 ) =   669
      HASH (  2548 ) = -2545
      HASH (  2549 ) =    25
      HASH (  2550 ) =  2357
      HASH (  2551 ) =   929
      HASH (  2552 ) = -2549
      HASH (  2553 ) =    23
      HASH (  2555 ) =  1866
      HASH (  2556 ) = -2553
      HASH (  2557 ) =    28
      HASH (  2558 ) =  2311
      HASH (  2559 ) =   792
      HASH (  2560 ) = -2557
      HASH (  2561 ) =    23
      HASH (  2563 ) =  1810
      HASH (  2564 ) = -2561
      HASH (  2565 ) =  1047
      HASH (  2566 ) =  2563
      HASH (  2567 ) =  1810
      HASH (  2568 ) = -2565
      HASH (  2569 ) =    13
      HASH (  2570 ) =  2143
      HASH (  2571 ) =  1563
      HASH (  2572 ) = -2569
      HASH (  2573 ) =    27
      HASH (  2575 ) =   785
      HASH (  2576 ) = -2573
      HASH (  2577 ) =  1367
      HASH (  2579 ) =   920
      HASH (  2580 ) = -2577
      HASH (  2581 ) =    13
      HASH (  2583 ) =  1943
      HASH (  2584 ) = -2581
      HASH (  2585 ) =   923
      HASH (  2586 ) =  2543
      HASH (  2587 ) =  1874
      HASH (  2588 ) = -2585
      HASH (  2589 ) =   919
      HASH (  2590 ) = -2587
      HASH (  2591 ) =  1757
      HASH (  2593 ) =  1422
      HASH (  2595 ) =  1558
      HASH (  2596 ) =  2375
      HASH (  2597 ) =  1485
      HASH (  2598 ) = -2595
      HASH (  2599 ) =  1738
      HASH (  2600 ) = -2597
      HASH (  2601 ) =    29
      HASH (  2603 ) =  1103
      HASH (  2604 ) = -2601
      HASH (  2605 ) =  1180
      HASH (  2606 ) = -2603
      HASH (  2607 ) =  1485
      HASH (  2609 ) =    27
      HASH (  2611 ) =  1565
      HASH (  2612 ) =  2405
      HASH (  2613 ) =  1175
      HASH (  2614 ) = -2611
      HASH (  2615 ) =    28
      HASH (  2617 ) =    15
      HASH (  2618 ) =  2579
      HASH (  2619 ) =   665
      HASH (  2620 ) =  2131
      HASH (  2621 ) =  1883
      HASH (  2622 ) = -2619
      HASH (  2623 ) =  1365
      HASH (  2625 ) =  2066
      HASH (  2626 ) = -2623
      HASH (  2627 ) =  1821
      HASH (  2628 ) = -2625
      HASH (  2629 ) =  1354
      HASH (  2630 ) = -2627
      HASH (  2631 ) =    21
      HASH (  2632 ) =  2629
      HASH (  2633 ) =  1173
      HASH (  2634 ) = -2631
      HASH (  2635 ) =  2976
      HASH (  2636 ) = -2633
      HASH (  2637 ) =  1821
      HASH (  2638 ) = -2635
      HASH (  2639 ) =  1485
      HASH (  2641 ) =    14
      HASH (  2642 ) =  2249
      HASH (  2643 ) =   669
      HASH (  2644 ) = -2641
      HASH (  2645 ) =  1175
      HASH (  2646 ) = -2643
      HASH (  2647 ) =  1750
      HASH (  2648 ) = -2645
      HASH (  2649 ) =  1934
      HASH (  2650 ) =  2559
      HASH (  2651 ) =  1175
      HASH (  2652 ) = -2649
      HASH (  2653 ) =  1501
      HASH (  2654 ) = -2651
      HASH (  2655 ) =   792
      HASH (  2656 ) = -2653
      HASH (  2657 ) =    14
      HASH (  2658 ) =  2599
      HASH (  2659 ) =  1180
      HASH (  2660 ) = -2657
      HASH (  2661 ) =   661
      HASH (  2662 ) =  2567
      HASH (  2663 ) =  1047
      HASH (  2664 ) = -2661
      HASH (  2665 ) =  1756
      HASH (  2666 ) =  2293
      HASH (  2667 ) =  1358
      HASH (  2668 ) = -2665
      HASH (  2669 ) =  1485
      HASH (  2670 ) = -2667
      HASH (  2671 ) =    23
      HASH (  2673 ) =   917
      HASH (  2674 ) = -2671
      HASH (  2675 ) =  1181
      HASH (  2676 ) = -2673
      HASH (  2677 ) =  2075
      HASH (  2678 ) = -2675
      HASH (  2679 ) =    14
      HASH (  2680 ) =  2677
      HASH (  2681 ) =  1181
      HASH (  2682 ) = -2679
      HASH (  2683 ) =    14
      HASH (  2685 ) =  1173
      HASH (  2686 ) = -2683
      HASH (  2687 ) =  2065
      HASH (  2688 ) = -2685
      HASH (  2689 ) =   919
      HASH (  2690 ) =  2687
      HASH (  2691 ) =  1181
      HASH (  2692 ) =  2443
      HASH (  2693 ) =    21
      HASH (  2694 ) =  2195
      HASH (  2695 ) =  1930
      HASH (  2696 ) = -2693
      HASH (  2697 ) =  1757
      HASH (  2698 ) = -2695
      HASH (  2699 ) =    27
      HASH (  2701 ) =    27
      HASH (  2702 ) =  2419
      HASH (  2703 ) =  2062
      HASH (  2704 ) = -2701
      HASH (  2705 ) =  1368
      HASH (  2706 ) = -2703
      HASH (  2707 ) =    27
      HASH (  2708 ) =  2421
      HASH (  2709 ) =  1614
      HASH (  2710 ) = -2707
      HASH (  2711 ) =   788
      HASH (  2712 ) =  2413
      HASH (  2713 ) =  1368
      HASH (  2714 ) = -2711
      HASH (  2715 ) =  1943
      HASH (  2716 ) = -2713
      HASH (  2717 ) =  1181
      HASH (  2718 ) =  2715
      HASH (  2719 ) =  1614
      HASH (  2720 ) =  1995
      HASH (  2721 ) =  1934
      HASH (  2722 ) =  2621
      HASH (  2723 ) =  1180
      HASH (  2725 ) =  1873
      HASH (  2726 ) = -2723
      HASH (  2727 ) =   919
      HASH (  2728 ) =  2725
      HASH (  2729 ) =    23
      HASH (  2730 ) =  2555
      HASH (  2731 ) =  1294
      HASH (  2732 ) = -2729
      HASH (  2733 ) =  1488
      HASH (  2734 ) =  2447
      HASH (  2735 ) =  1746
      HASH (  2736 ) = -2733
      HASH (  2737 ) =  1821
      HASH (  2738 ) = -2735
      HASH (  2739 ) =  1561
      HASH (  2740 ) =  2737
      HASH (  2741 ) =  1821
      HASH (  2742 ) = -2739
      HASH (  2743 ) =   921
      HASH (  2744 ) =  2741
      HASH (  2745 ) =    14
      HASH (  2746 ) =  2477
      HASH (  2747 ) =  1563
      HASH (  2748 ) = -2745
      HASH (  2749 ) =  1617
      HASH (  2750 ) = -2747
      HASH (  2751 ) =  1418
      HASH (  2752 ) = -2749
      HASH (  2753 ) =  1751
      HASH (  2754 ) =  2535
      HASH (  2755 ) =  1886
      HASH (  2756 ) = -2753
      HASH (  2757 ) =  1742
      HASH (  2758 ) = -2755
      HASH (  2759 ) =  1422
      HASH (  2760 ) =  2757
      HASH (  2761 ) =  1822
      HASH (  2762 ) = -2759
      HASH (  2763 ) =  1742
      HASH (  2764 ) = -2761
      HASH (  2765 ) =  1373
      HASH (  2766 ) =  2763
      HASH (  2767 ) =  1822
      HASH (  2768 ) = -2765
      HASH (  2769 ) =  1742
      HASH (  2770 ) = -2767
      HASH (  2771 ) =   661
      HASH (  2772 ) =  2769
      HASH (  2773 ) =  1742
      HASH (  2774 ) = -2771
      HASH (  2775 ) =   653
      HASH (  2776 ) =  2773
      HASH (  2777 ) =    10
      HASH (  2778 ) =  2033
      HASH (  2779 ) =    28
      HASH (  2780 ) =  2469
      HASH (  2781 ) =   924
      HASH (  2782 ) = -2779
      HASH (  2783 ) =  1548
      HASH (  2784 ) = -2781
      HASH (  2785 ) =  1627
      HASH (  2786 ) = -2783
      HASH (  2787 ) =    14
      HASH (  2788 ) =  2785
      HASH (  2789 ) =  1947
      HASH (  2790 ) = -2787
      HASH (  2791 ) =   909
      HASH (  2792 ) = -2789
      HASH (  2793 ) =  1548
      HASH (  2794 ) = -2791
      HASH (  2795 ) =  1627
      HASH (  2796 ) = -2793
      HASH (  2797 ) =   911
      HASH (  2798 ) =  2795
      HASH (  2799 ) =    15
      HASH (  2800 ) =  2571
      HASH (  2801 ) =  2962
      HASH (  2802 ) = -2799
      HASH (  2803 ) =  1563
      HASH (  2804 ) = -2801
      HASH (  2805 ) =  1949
      HASH (  2806 ) =  2113
      HASH (  2807 ) =  1881
      HASH (  2808 ) = -2805
      HASH (  2809 ) =    14
      HASH (  2810 ) =  2387
      HASH (  2811 ) =  1180
      HASH (  2812 ) = -2809
      HASH (  2813 ) =  1760
      HASH (  2814 ) = -2811
      HASH (  2815 ) =  1102
      HASH (  2816 ) = -2813
      HASH (  2817 ) =  1167
      HASH (  2818 ) =  2803
      HASH (  2819 ) =  1563
      HASH (  2820 ) = -2817
      HASH (  2821 ) =    29
      HASH (  2822 ) =  2029
      HASH (  2823 ) =  1496
      HASH (  2824 ) = -2821
      HASH (  2825 ) =  1486
      HASH (  2826 ) =  2823
      HASH (  2827 ) =    32
      HASH (  2829 ) =    13
      HASH (  2830 ) =  2279
      HASH (  2831 ) =  1175
      HASH (  2833 ) =   788
      HASH (  2834 ) =  2705
      HASH (  2835 ) =  1365
      HASH (  2837 ) =    29
      HASH (  2838 ) =  2613
      HASH (  2839 ) =  1566
      HASH (  2840 ) = -2837
      HASH (  2841 ) =  1175
      HASH (  2842 ) = -2839
      HASH (  2843 ) =    27
      HASH (  2844 ) =  2841
      HASH (  2845 ) =  1038
      HASH (  2846 ) = -2843
      HASH (  2847 ) =  1870
      HASH (  2848 ) = -2845
      HASH (  2849 ) =  1175
      HASH (  2850 ) = -2847
      HASH (  2851 ) =    29
      HASH (  2852 ) =  2849
      HASH (  2853 ) =  1630
      HASH (  2854 ) = -2851
      HASH (  2855 ) =  1175
      HASH (  2856 ) = -2853
      HASH (  2857 ) =    27
      HASH (  2858 ) =  2855
      HASH (  2859 ) =  1486
      HASH (  2860 ) = -2857
      HASH (  2861 ) =  1175
      HASH (  2862 ) = -2859
      HASH (  2863 ) =   919
      HASH (  2865 ) =   845
      HASH (  2866 ) = -2863
      HASH (  2867 ) =  1559
      HASH (  2868 ) =  2237
      HASH (  2869 ) =  1874
      HASH (  2870 ) = -2867
      HASH (  2871 ) =  1484
      HASH (  2872 ) = -2869
      HASH (  2873 ) =    27
      HASH (  2875 ) =    14
      HASH (  2877 ) =  1372
      HASH (  2878 ) = -2875
      HASH (  2879 ) =   661
      HASH (  2880 ) =  2551
      HASH (  2881 ) =  1751
      HASH (  2882 ) = -2879
      HASH (  2883 ) =  1870
      HASH (  2884 ) = -2881
      HASH (  2885 ) =   929
      HASH (  2886 ) = -2883
      HASH (  2887 ) =  1181
      HASH (  2888 ) =  2885
      HASH (  2889 ) =   652
      HASH (  2891 ) =    13
      HASH (  2892 ) =  2589
      HASH (  2893 ) =  1806
      HASH (  2895 ) =  1880
      HASH (  2897 ) =  2071
      HASH (  2898 ) = -2895
      HASH (  2899 ) =   856
      HASH (  2900 ) = -2897
      HASH (  2901 ) =    31
      HASH (  2902 ) =  2097
      HASH (  2903 ) =   850
      HASH (  2904 ) = -2901
      HASH (  2905 ) =    22
      HASH (  2906 ) =  2903
      HASH (  2907 ) =   785
      HASH (  2908 ) =  2393
      HASH (  2909 ) =  1866
      HASH (  2910 ) = -2907
      HASH (  2911 ) =    14
      HASH (  2912 ) =  2655
      HASH (  2913 ) =  1175
      HASH (  2914 ) = -2911
      HASH (  2915 ) =  1949
      HASH (  2916 ) = -2913
      HASH (  2917 ) =  1752
      HASH (  2918 ) = -2915
      HASH (  2919 ) =   792
      HASH (  2920 ) = -2917
      HASH (  2921 ) =  1634
      HASH (  2922 ) =  2919
      HASH (  2923 ) =   792
      HASH (  2924 ) = -2921
      HASH (  2925 ) =    29
      HASH (  2926 ) =  2923
      HASH (  2927 ) =  1500
      HASH (  2928 ) = -2925
      HASH (  2929 ) =   667
      HASH (  2930 ) =  2575
      HASH (  2931 ) =   785
      HASH (  2932 ) = -2929
      HASH (  2933 ) =    14
      HASH (  2934 ) =  2369
      HASH (  2935 ) =  1564
      HASH (  2936 ) = -2933
      HASH (  2937 ) =   789
      HASH (  2938 ) = -2935
      HASH (  2939 ) =    28
      HASH (  2940 ) =  2937
      HASH (  2941 ) =   668
      HASH (  2942 ) = -2939
      HASH (  2943 ) =    27
      HASH (  2944 ) =  2931
      HASH (  2945 ) =  1870
      HASH (  2946 ) = -2943
      HASH (  2947 ) =   652
      HASH (  2948 ) = -2945
      HASH (  2949 ) =   667
      HASH (  2950 ) = -2947
      HASH (  2951 ) =  1806
      HASH (  2953 ) =   778
      HASH (  2954 ) = -2951
      HASH (  2955 ) =  1365
      HASH (  2956 ) =  2953
      HASH (  2957 ) =  1557
      HASH (  2958 ) =  2201
      HASH (  2959 ) =   728
      HASH (  2960 ) = -2957
      HASH (  2961 ) =    23
      HASH (  2962 ) =  2959
      HASH (  2963 ) =   906
      HASH (  2964 ) = -2961
      HASH (  2965 ) =  1557
      HASH (  2966 ) = -2963
      HASH (  2967 ) =    20
      HASH (  2969 ) =  1548
      HASH (  2970 ) = -2967
      HASH (  2971 ) =    23
      HASH (  2973 ) =  1042
      HASH (  2974 ) = -2971
      HASH (  2975 ) =  1551
      HASH (  2977 ) =  2222
      HASH (  2978 ) = -2975
      HASH (  2979 ) =  1738
      HASH (  2980 ) = -2977
      HASH (  2981 ) =   667
      HASH (  2982 ) = -2979
      HASH (  2983 ) =   785
      HASH (  2984 ) =  2547
      HASH (  2985 ) =  1866
      HASH (  2986 ) = -2983
      HASH (  2987 ) =    15
      HASH (  2988 ) =  2981
      HASH (  2989 ) =  2200
      HASH (  2990 ) = -2987
      HASH (  2991 ) =  1738
      HASH (  2992 ) = -2989
      HASH (  2993 ) =   667
      HASH (  2994 ) = -2991
      HASH (  2995 ) =    34
      HASH (  2996 ) =  2993
      HASH (  2997 ) =  1738
      HASH (  2998 ) = -2995
      HASH (  2999 ) =    13
      HASH (  3000 ) =  2505
      
      return
      end
      
      
      INTEGER*2 FUNCTION SEARCH(K,name,hash,nlast)
      IMPLICIT INTEGER*2 (A-Z)
      dimension hash(8000), name(10)
      
      m = 1009
      NAME1=NAME(1)
      I=MOD(NAME1,M)
      I=I*2+1
      IF (HASH(I).NE.0) GOTO 3
      SEARCH=I
      HASH(I)=NAME1
      GOTO 11
1     IF (I.NE.0) GOTO 3
2     NLAST=NLAST-2
      IF (NLAST.LT.0) write(*,99)
 99   format('  przepelnienie')     
      IF (HASH(NLAST).NE.0) GOTO 2
      SEARCH=NLAST
      HASH(NLAST)=NAME(1)
      HASH(J+1)=NLAST
      I=NLAST
      GOTO 11
3     IF (HASH(I).EQ.NAME1) GOTO 4
      J=I
      I=HASH(J+1)
      GOTO 10
4     IF ((K.NE.1).OR.(HASH(I+1).LT.0)) GOTO 5
      SEARCH=I
      RETURN
5     J=I
      P=1
6     T=HASH(J+1)
      P=P+1
      IF (P.LE.K) GOTO 8
      IF (T.LT.0) GOTO 7
      SEARCH=I
      RETURN
7     J=-T
      I=HASH(J+1)
      GOTO 10
8     IF (T.LT.0) GOTO 9
      I=T
      GOTO 1
9     J=-T
      IF (NAME(P).EQ.HASH(J)) GOTO 6
      I=HASH(J+1)
10    IF (I.GE.0) GOTO 1
      J=-I
      I=HASH(J+1)
      GOTO 10
11    P=2
12    IF (P.GT.K) RETURN
13    NLAST=NLAST-2
      IF (NLAST.LT.0) write(*,99)
      IF (HASH(NLAST).NE.0) GOTO 13
      HASH(NLAST)=NAME(P)
      HASH(I+1)=-NLAST
      I=NLAST
      P=P+1
      GOTO 12
      END