const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardReleaseOptions();

    const exe_cflags = [_][]const u8{
        "-std=c++14",
        "-D__STDC_CONSTANT_MACROS",
        "-D__STDC_FORMAT_MACROS",
        "-D__STDC_LIMIT_MACROS",
        "-D_GNU_SOURCE",
        "-fvisibility-inlines-hidden",
        "-fno-rtti",
        "-Werror=type-limits",
        "-Wno-missing-braces",
        "-Wno-comment",
    };

    const exe = b.addExecutable("ec", "src/main.cpp");
    exe.setTarget(target);
    exe.setBuildMode(mode);
    exe.install();
    exe.addCSourceFiles(&.{"src/Tokenizer.cpp"}, &exe_cflags);
    exe.linkLibCpp();

    const run_cmd = exe.run();
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
