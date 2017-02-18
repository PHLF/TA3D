


#
# --- Applying the Configuation ---
#

set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "")

#
# MMX
#
if("${YUNI_PROFILE_MMX}" STREQUAL "yes")
	if(CMAKE_COMPILER_IS_GNUCXX)
		set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS} -mmmx")
	else()
	endif()
endif()

#
# 3D NOW
#
if("${YUNI_PROFILE_3DNOW}" STREQUAL "yes")
	if(CMAKE_COMPILER_IS_GNUCXX)
		set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS} -m3dnow")
	else()
	endif()
endif()



#
# SSE
#
if("${YUNI_PROFILE_SSE}" STREQUAL "yes")
	if(CMAKE_COMPILER_IS_GNUCXX)
		set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS} -msse -mfpmath=sse")
	else()
		set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS} -msse -mfpmath=sse")
	endif()

	# SSE2
	if("${YUNI_PROFILE_SSE2}" STREQUAL "yes")
		if(CMAKE_COMPILER_IS_GNUCXX)
			set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS} -msse2")
		else()
			if(MSVC)
				set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS} /arch:SSE2")
			endif()
		endif()

		# SSE3
		if("${YUNI_PROFILE_SSE3}" STREQUAL "yes")
			if(CMAKE_COMPILER_IS_GNUCXX)
				set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS} -msse3")
			else()
			endif()

			# SSE4
			if("${YUNI_PROFILE_SSE4}" STREQUAL "yes")
				if(CMAKE_COMPILER_IS_GNUCXX)
					set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS} -msse4")
				else()
				endif()
			endif()

			# SSE4a
			if("${YUNI_PROFILE_SSE4a}" STREQUAL "yes")
				if(CMAKE_COMPILER_IS_GNUCXX)
					set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS} -msse4a")
				else()
				endif()
			endif()

			# SSE 4.1
			if("${YUNI_PROFILE_SSE4_1}" STREQUAL "yes")
				if(CMAKE_COMPILER_IS_GNUCXX)
					set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS} -msse4.1")
				else()
				endif()
			endif()

			# SSE 4.2
			if("${YUNI_PROFILE_SSE4_2}" STREQUAL "yes")
				if(CMAKE_COMPILER_IS_GNUCXX)
					set(YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS "${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS} -msse4.2")
				else()
				endif()
			endif()

		endif()

	endif()

endif()

