defmodule HexOutput do
  def write_custom_output(file_path) do
    target_values = [2, 4, 6, 8, 12, 14, 16, 58, 78, 98, 118, 138, 158, 178]

    result =
      for m0 <- 0..15, p0 <- 0..3 do
        lm_results =
          for lm <- 0..198, rem(lm, 2) == 0 do
            hex_value =
              if lm in target_values do
                "0000000000000001"
              else
                "0000000000000000"
              end

            "d set lm#{lm}m#{m0}p#{p0} #{hex_value}"
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
