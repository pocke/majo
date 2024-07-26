module Majo
  class Result
    def report(out: $stdout, formatter: nil)
      fmt =
        case formatter
        when :color
          Formatter::Color
        when :csv
          Formatter::CSV
        when :monochrome
          Formatter::Monochrome
        when nil
          if out.respond_to?(:tty?) && out.tty?
            Formatter::Color
          else
            Formatter::Monochrome
          end
        else
          raise "unknown formatter: #{formatter.inspect}"
        end

      with_output(out) do |io|
        io.puts fmt.new(self).call
      end
    end

    private

    def with_output(out)
      case
      when out.is_a?(IO) || (defined?(StringIO) && out.is_a?(StringIO))
        yield out
      when out.is_a?(String)
        File.open(out, "w") { |io| yield io }
      when out.respond_to?(:to_path)
        File.open(out.to_path, "w") { |io| yield io }
      else
        raise ArgumentError, "out must be an IO or a String"
      end
    end
  end
end
