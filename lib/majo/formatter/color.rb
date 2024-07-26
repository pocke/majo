module Majo
  module Formatter
    class Color
      include Colorize

      def initialize(result)
        @result = result
      end

      def call
        allocs = result.allocations
        retained = result.retained

        <<~RESULT
          Total #{total_memory(allocs)} bytes (#{total_objects(allocs)} objects)
          Total retained #{total_memory(retained)} bytes (#{total_objects(retained)} objects)

          Memory by file
          #{bar}
          #{format_two_columns(memory_by_file(allocs))}

          Memory by location
          #{bar}
          #{format_two_columns(memory_by_location(allocs))}

          Memory by class
          #{bar}
          #{format_two_columns(memory_by_class(allocs))}

          Objects by file
          #{bar}
          #{format_two_columns(objects_by_file(allocs))}

          Objects by location
          #{bar}
          #{format_two_columns(objects_by_location(allocs))}

          Objects by class
          #{bar}
          #{format_two_columns(objects_by_class(allocs))}

          Retained Memory by file
          #{bar}
          #{format_two_columns(memory_by_file(retained))}

          Retained Memory by location
          #{bar}
          #{format_two_columns(memory_by_location(retained))}

          Retained Memory by class
          #{bar}
          #{format_two_columns(memory_by_class(retained))}

          Retained Objects by file
          #{bar}
          #{format_two_columns(objects_by_file(retained))}

          Retained Objects by location
          #{bar}
          #{format_two_columns(objects_by_location(retained))}

          Retained Objects by class
          #{bar}
          #{format_two_columns(objects_by_class(retained))}
        RESULT
      end

      private

      attr_reader :result

      def bar
        cyan '-----------------------------------'
      end

      def total_objects(allocs)
        allocs.size
      end

      def total_memory(allocs)
        allocs.sum(&:memsize)
      end

      def memory_by_file(allocs)
        allocs.group_by(&:path).map do |path, allocations|
          [allocations.sum(&:memsize), path]
        end.sort_by(&:first).reverse
      end

      def memory_by_location(allocs)
        allocs.group_by { |a| "#{a.path}:#{a.line}" }.map do |location, allocations|
          [allocations.sum(&:memsize), location]
        end.sort_by(&:first).reverse
      end

      def memory_by_class(allocs)
        allocs.group_by(&:object_class_path).map do |class_path, allocations|
          [allocations.sum(&:memsize), class_path]
        end.sort_by(&:first).reverse
      end

      def objects_by_file(allocs)
        allocs.group_by(&:path).map do |path, allocations|
          [allocations.size, path]
        end.sort_by(&:first).reverse
      end

      def objects_by_location(allocs)
        allocs.group_by { |a| "#{a.path}:#{a.line}" }.map do |location, allocations|
          [allocations.size, location]
        end.sort_by(&:first).reverse
      end

      def objects_by_class(allocs)
        allocs.group_by(&:object_class_path).map do |class_path, allocations|
          [allocations.size, class_path]
        end.sort_by(&:first).reverse
      end

      def format_two_columns(data)
        return "" if data.empty?

        max_length = data.max_by { |row| row[0].to_s.size }[0].to_s.size
        data.map { |row| "#{blue(row[0].to_s.rjust(max_length))}  #{row[1]}" }.join("\n")
      end
    end
  end
end
