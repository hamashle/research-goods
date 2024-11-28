defmodule HexOutput do
  def write_custom_output(file_path) do
    result =
      for m0 <- 0..15, p0 <- 0..3 do
        lm_results =
          for lm <- 0..198, rem(lm, 2) == 0 do
            "d get $lm#{lm}n0c0b0m#{m0}p#{p0} 1"
          end

        Enum.join(lm_results, "\n") <> "\n"
      end
      |> Enum.join("\n")

    File.write!(file_path, result)
    IO.puts("結果が #{file_path} に保存されました。")
  end
end

file_path = "output.txt"
HexOutput.write_custom_output(file_path)
