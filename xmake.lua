add_rules("mode.debug", "mode.release")

add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")
if is_plat("windows") then
    add_requires("detours v4.0.1-xmake.1")
elseif is_plat("android") then   
    add_requires("dobby")
end
add_requires("nlohmann_json v3.11.3")

target("ForceCloseOreUI")
    set_kind("shared")
    add_files("src/**.cpp")
    add_includedirs("src")
    set_languages("c++20")
    set_strip("all")
    add_packages("nlohmann_json")
    if is_plat("windows") then
        add_packages("detours")
        remove_files("src/api/memory/android/**.cpp","src/api/memory/android/**.h")
        add_cxflags("/utf-8", "/EHa")
    elseif is_plat("android") then
        add_packages("dobby")
        remove_files("src/api/memory/win/**.cpp","src/api/memory/win/**.h")
        add_cxflags("-O3")
    end