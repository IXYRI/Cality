set_project("cality")
set_languages("c17")
set_warnings("all", "extra")

add_includedirs("src")
local units = os.files("src/*.c")
for i, f in ipairs(units) do
	units[i] = path.basename(f)
	target(units[i])
		set_kind("static")
		add_files(f)
end

target("cality")
	add_headerfiles("src/*.h")
	set_kind("static")
	add_deps(units)

for _, tp in ipairs(os.files("tests/*.c")) do
	local test = path.basename(tp)
	target(test)
		add_deps("cality")
		add_tests(test)
		add_files(tp)
end
