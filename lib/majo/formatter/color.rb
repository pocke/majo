module Majo
  module Formatter
    class Color
      include Colorize

      def initialize(result)
        @result = result
      end

      def call
        <<~RESULT
          Total #{total_memory} bytes (#{total_objects} objects)

          Memory by file
          #{bar}
          #{format_two_columns(memory_by_file)}

          Memory by location
          #{bar}
          #{format_two_columns(memory_by_location)}

          Memory by class
          #{bar}
          #{format_two_columns(memory_by_class)}

          Objects by file
          #{bar}
          #{format_two_columns(objects_by_file)}

          Objects by location
          #{bar}
          #{format_two_columns(objects_by_location)}

          Objects by class
          #{bar}
          #{format_two_columns(objects_by_class)}
        RESULT
      end

      private

      attr_reader :result

      def bar
        cyan '-----------------------------------'
      end

      def total_objects
        allocs.size
      end

      def total_memory
        allocs.sum(&:memsize)
      end

      def memory_by_file
        allocs.group_by(&:path).map do |path, allocations|
          [allocations.sum(&:memsize), path]
        end.sort_by(&:first).reverse
      end

      def memory_by_location
        allocs.group_by { |a| "#{a.path}:#{a.line}" }.map do |location, allocations|
          [allocations.sum(&:memsize), location]
        end.sort_by(&:first).reverse
      end

      def memory_by_class
        allocs.group_by(&:object_class_path).map do |class_path, allocations|
          [allocations.sum(&:memsize), class_path]
        end.sort_by(&:first).reverse
      end

      def objects_by_file
        allocs.group_by(&:path).map do |path, allocations|
          [allocations.size, path]
        end.sort_by(&:first).reverse
      end

      def objects_by_location
        allocs.group_by { |a| "#{a.path}:#{a.line}" }.map do |location, allocations|
          [allocations.size, location]
        end.sort_by(&:first).reverse
      end

      def objects_by_class
        allocs.group_by(&:object_class_path).map do |class_path, allocations|
          [allocations.size, class_path]
        end.sort_by(&:first).reverse
      end

      def format_two_columns(data)
        return "" if data.empty?

        max_length = data.max_by { |row| row[0].to_s.size }[0].to_s.size
        data.map { |row| "#{blue(row[0].to_s.rjust(max_length))}  #{row[1]}" }.join("\n")
      end

      def allocs
        @allocs ||= result.allocations
      end
    end
  end
end
