# Function for selective cloning/downloading files from Git repository
# Cross-platform support for Windows, Linux, and macOS
# Supports both git sparse checkout (if available) and archive download fallback
# Added file type filtering support
function(fetch_git_partial)
    # Define function parameters
    set(oneValueArgs GIT_URL TAG DEST_DIR TEMP_DIR)
    set(multiValueArgs SRC_PATHS FILE_PATTERNS)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # Validate required parameters
    if(NOT ARG_GIT_URL)
        message(FATAL_ERROR "GIT_URL parameter is required")
    endif()
    if(NOT ARG_TAG)
        message(FATAL_ERROR "TAG parameter is required")
    endif()
    if(NOT ARG_DEST_DIR)
        message(FATAL_ERROR "DEST_DIR parameter is required")
    endif()
    if(NOT ARG_TEMP_DIR)
        message(FATAL_ERROR "TEMP_DIR parameter is required")
    endif()
    if(NOT ARG_SRC_PATHS)
        message(FATAL_ERROR "SRC_PATHS parameter is required")
    endif()
    
    # Set default file patterns if not specified
    if(NOT ARG_FILE_PATTERNS)
        set(ARG_FILE_PATTERNS "*")  # Copy everything by default
    endif()
    
    # Create temporary directory for operations
    file(MAKE_DIRECTORY ${ARG_TEMP_DIR})
    
    # Check if git is available in the system
    find_program(GIT_EXECUTABLE git)
    if(GIT_EXECUTABLE)
        message(STATUS "Using git sparse checkout for efficient download")
        fetch_with_git("${ARG_GIT_URL}" "${ARG_TAG}" "${ARG_SRC_PATHS}" "${ARG_DEST_DIR}" "${ARG_TEMP_DIR}" "${ARG_FILE_PATTERNS}")
    else()
        message(STATUS "Git not available, falling back to archive download")
        fetch_with_archive("${ARG_GIT_URL}" "${ARG_TAG}" "${ARG_SRC_PATHS}" "${ARG_DEST_DIR}" "${ARG_TEMP_DIR}" "${ARG_FILE_PATTERNS}")
    endif()
endfunction()

# Internal function: Git-based implementation using sparse checkout
function(fetch_with_git git_url tag src_paths dest_dir temp_dir file_patterns)
    # Clean temporary directory
    file(REMOVE_RECURSE ${temp_dir})
    file(MAKE_DIRECTORY ${temp_dir})
    
    # Initialize temporary git repository
    execute_process(
        COMMAND ${GIT_EXECUTABLE} init
        WORKING_DIRECTORY ${temp_dir}
        RESULT_VARIABLE result
        OUTPUT_QUIET
        ERROR_QUIET
    )
    if(result)
        message(FATAL_ERROR "Failed to initialize git repository")
    endif()
    
    # Add remote origin to the repository
    execute_process(
        COMMAND ${GIT_EXECUTABLE} remote add origin ${git_url}
        WORKING_DIRECTORY ${temp_dir}
        RESULT_VARIABLE result
        OUTPUT_QUIET
        ERROR_QUIET
    )
    if(result)
        message(FATAL_ERROR "Failed to add remote origin")
    endif()
    
    # Enable sparse checkout feature
    execute_process(
        COMMAND ${GIT_EXECUTABLE} config core.sparseCheckout true
        WORKING_DIRECTORY ${temp_dir}
        RESULT_VARIABLE result
        OUTPUT_QUIET
        ERROR_QUIET
    )
    if(result)
        message(FATAL_ERROR "Failed to enable sparse checkout")
    endif()
    
    # Create sparse-checkout configuration file
    set(sparse_file "${temp_dir}/.git/info/sparse-checkout")
    file(WRITE ${sparse_file} "")
    foreach(path IN LISTS src_paths)
        # Normalize paths for cross-platform compatibility
        file(TO_CMAKE_PATH "${path}" normalized_path)
        file(APPEND ${sparse_file} "${normalized_path}\n")
    endforeach()
    
    # Fetch the specific tag/branch from remote
    execute_process(
        COMMAND ${GIT_EXECUTABLE} fetch --depth 1 origin ${tag}
        WORKING_DIRECTORY ${temp_dir}
        RESULT_VARIABLE result
        OUTPUT_QUIET
        ERROR_QUIET
    )
    
    # If shallow fetch failed, try full fetch
    if(result)
        message(STATUS "Shallow fetch failed, trying full fetch...")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} fetch origin ${tag}
            WORKING_DIRECTORY ${temp_dir}
            RESULT_VARIABLE result
            OUTPUT_QUIET
            ERROR_QUIET
        )
        if(result)
            message(FATAL_ERROR "Failed to fetch from git repository")
        endif()
    endif()
    
    # Checkout the fetched content
    execute_process(
        COMMAND ${GIT_EXECUTABLE} checkout FETCH_HEAD
        WORKING_DIRECTORY ${temp_dir}
        RESULT_VARIABLE result
        OUTPUT_QUIET
        ERROR_QUIET
    )
    if(result)
        message(FATAL_ERROR "Failed to checkout ${tag}")
    endif()
    
    # Copy requested files/directories to destination with file filtering
    foreach(src_path IN LISTS src_paths)
        set(src_full_path "${temp_dir}/${src_path}")
        set(dest_full_path "${dest_dir}/${src_path}")
        
        if(EXISTS ${src_full_path})
            if(IS_DIRECTORY ${src_full_path})
                # Copy directory with file filtering
                copy_directory_with_filter("${src_full_path}" "${dest_full_path}" "${file_patterns}")
            else()
                # Copy single file if it matches patterns
                copy_file_if_matches("${src_full_path}" "${dest_full_path}" "${file_patterns}")
            endif()
        else()
            message(WARNING "Requested path ${src_path} not found in repository")
        endif()
    endforeach()
    
    # Clean up temporary directory
    file(REMOVE_RECURSE ${temp_dir})
endfunction()

# Internal function: Fallback implementation using archive download
function(fetch_with_archive git_url tag src_paths dest_dir temp_dir file_patterns)
    # Clean temporary directory
    file(REMOVE_RECURSE ${temp_dir})
    file(MAKE_DIRECTORY ${temp_dir})
    
    # Detect repository hosting service for proper archive URL format
    if(git_url MATCHES "github\\.com")
        string(REGEX REPLACE "\\.git$" "" repo_base ${git_url})
        set(archive_url "${repo_base}/archive/${tag}.tar.gz")
    elseif(git_url MATCHES "gitlab\\.com")
        string(REGEX REPLACE "\\.git$" "" repo_base ${git_url})
        get_filename_component(repo_name ${repo_base} NAME)
        set(archive_url "${repo_base}/-/archive/${tag}/${repo_name}-${tag}.tar.gz")
    elseif(git_url MATCHES "bitbucket\\.org")
        string(REGEX REPLACE "\\.git$" "" repo_base ${git_url})
        set(archive_url "${repo_base}/get/${tag}.tar.gz")
    else()
        string(REGEX REPLACE "\\.git$" "" repo_base ${git_url})
        set(archive_url "${repo_base}/archive/${tag}.tar.gz")
    endif()
    
    # Download the archive file
    set(archive_path "${temp_dir}/archive.tar.gz")
    message(STATUS "Downloading archive from ${archive_url}")
    
    # Try different archive extensions
    set(archive_extensions .tar.gz .tgz .zip .tar.bz2)
    set(download_success FALSE)
    
    foreach(ext ${archive_extensions})
        string(REPLACE ".tar.gz" "${ext}" try_archive_url ${archive_url})
        message(STATUS "Trying ${try_archive_url}")
        
        file(DOWNLOAD
            ${try_archive_url}
            ${archive_path}
            TIMEOUT 60
            TLS_VERIFY ON
            STATUS download_status
        )
        
        list(GET download_status 0 status_code)
        if(status_code EQUAL 0)
            set(download_success TRUE)
            set(archive_url ${try_archive_url})
            break()
        endif()
    endforeach()
    
    if(NOT download_success)
        message(FATAL_ERROR "Failed to download archive from any known format")
    endif()
    
    # Extract the downloaded archive
    set(extracted_dir "${temp_dir}/extracted")
    file(MAKE_DIRECTORY ${extracted_dir})
    
    # Determine archive type and extract accordingly
    if(archive_path MATCHES "\\.(tar\\.gz|tgz)$")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf ${archive_path}
            WORKING_DIRECTORY ${extracted_dir}
            RESULT_VARIABLE result
        )
    elseif(archive_path MATCHES "\\.tar\\.bz2$")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xjf ${archive_path}
            WORKING_DIRECTORY ${extracted_dir}
            RESULT_VARIABLE result
        )
    elseif(archive_path MATCHES "\\.zip$")
        if(WIN32)
            find_program(UNZIP_EXECUTABLE unzip)
            if(UNZIP_EXECUTABLE)
                execute_process(
                    COMMAND ${UNZIP_EXECUTABLE} -q ${archive_path}
                    WORKING_DIRECTORY ${extracted_dir}
                    RESULT_VARIABLE result
                )
            else()
                execute_process(
                    COMMAND ${CMAKE_COMMAND} -E tar xzf ${archive_path}
                    WORKING_DIRECTORY ${extracted_dir}
                    RESULT_VARIABLE result
                )
            endif()
        else()
            execute_process(
                COMMARD unzip -q ${archive_path}
                WORKING_DIRECTORY ${extracted_dir}
                RESULT_VARIABLE result
            )
        endif()
    else()
        message(FATAL_ERROR "Unsupported archive format: ${archive_path}")
    endif()
    
    if(result)
        message(FATAL_ERROR "Failed to extract archive: ${result}")
    endif()
    
    # Find the extracted directory
    file(GLOB extracted_contents LIST_DIRECTORIES true "${extracted_dir}/*")
    list(LENGTH extracted_contents num_contents)
    
    if(num_contents EQUAL 0)
        message(FATAL_ERROR "No contents found in extracted archive")
    endif()
    
    # Use the first directory found
    list(GET extracted_contents 0 source_root)
    
    # Copy only the requested paths with file filtering
    foreach(src_path IN LISTS src_paths)
        set(src_full_path "${source_root}/${src_path}")
        set(dest_full_path "${dest_dir}/${src_path}")
        
        if(EXISTS ${src_full_path})
            if(IS_DIRECTORY ${src_full_path})
                # Copy directory with file filtering
                copy_directory_with_filter("${src_full_path}" "${dest_full_path}" "${file_patterns}")
            else()
                # Copy single file if it matches patterns
                copy_file_if_matches("${src_full_path}" "${dest_full_path}" "${file_patterns}")
            endif()
        else()
            message(WARNING "Requested path ${src_path} not found in archive")
        endif()
    endforeach()
    
    # Clean up temporary directory
    file(REMOVE_RECURSE ${temp_dir})
endfunction()

# Helper function: Copy directory with file pattern filtering
function(copy_directory_with_filter src_dir dest_dir file_patterns)
    # Create destination directory
    file(MAKE_DIRECTORY ${dest_dir})
    
    # Handle wildcard pattern (copy everything)
    if("${file_patterns}" STREQUAL "*")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${src_dir} ${dest_dir}
            RESULT_VARIABLE result
        )
        if(NOT result)
            message(STATUS "Copied directory ${src_dir} to ${dest_dir}")
        else()
            message(WARNING "Failed to copy directory ${src_dir}")
        endif()
        return()
    endif()
    
    # Recursively find and copy files matching patterns
    foreach(pattern IN LISTS file_patterns)
        # Find all files matching the pattern
        file(GLOB_RECURSE matching_files 
            RELATIVE ${src_dir}
            "${src_dir}/${pattern}"
        )
        
        foreach(file_path IN LISTS matching_files)
            set(src_file "${src_dir}/${file_path}")
            set(dest_file "${dest_dir}/${file_path}")
            
            # Create destination directory
            get_filename_component(dest_file_dir ${dest_file} DIRECTORY)
            file(MAKE_DIRECTORY ${dest_file_dir})
            
            # Copy the file
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E copy ${src_file} ${dest_file}
                RESULT_VARIABLE result
            )
            
            if(result)
                message(WARNING "Failed to copy ${file_path}")
            endif()
        endforeach()
    endforeach()
endfunction()

# Helper function: Check if single file matches patterns and copy it
function(copy_file_if_matches src_file dest_file file_patterns)
    # Handle wildcard pattern (copy everything)
    if("${file_patterns}" STREQUAL "*")
        # Create destination directory
        get_filename_component(dest_dir ${dest_file} DIRECTORY)
        file(MAKE_DIRECTORY ${dest_dir})
        
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy ${src_file} ${dest_file}
            RESULT_VARIABLE result
        )

        if(result)
            message(WARNING "Failed to copy ${src_file}")
        endif()
        return()
    endif()
    
    # Check if file matches any of the patterns
    get_filename_component(file_name ${src_file} NAME)
    foreach(pattern IN LISTS file_patterns)
        if(file_name MATCHES "^${pattern}$" OR file_name MATCHES "${pattern}")
            # Create destination directory
            get_filename_component(dest_dir ${dest_file} DIRECTORY)
            file(MAKE_DIRECTORY ${dest_dir})
            
            # Copy the file
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E copy ${src_file} ${dest_file}
                RESULT_VARIABLE result
            )
            
            if(result)
                message(WARNING "Failed to copy ${src_file}")
            endif()
            return()
        endif()
    endforeach()
    
    message(STATUS "Skipped ${src_file} (does not match patterns)")
endfunction()
