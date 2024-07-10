module Majo
  class Result
    def report
      <<~RESULT
        Total #{total_memory} bytes (#{total_objects} objects)

        Memory by file
        #{format_two_columns(memory_by_file)}

        Memory by location
        #{format_two_columns(memory_by_location)}

        Memory by class
        #{format_two_columns(memory_by_class)}

        Objects by file
        #{format_two_columns(objects_by_file)}

        Objects by location
        #{format_two_columns(objects_by_location)}

        Objects by class
        #{format_two_columns(objects_by_class)}
      RESULT
    end

    private

    def total_objects
      allocs.size
    end

    def total_memory
      allocs.sum(&:memsize)
    end

    def memory_by_file
      allocs.group_by(&:path).map do |path, allocations|
        [allocations.sum(&:memsize).to_s, path]
      end
    end

    def memory_by_location
      allocs.group_by { |a| "#{a.path}:#{a.line}" }.map do |location, allocations|
        [allocations.sum(&:memsize).to_s, location]
      end
    end

    def memory_by_class
      allocs.group_by(&:object_class_path).map do |class_path, allocations|
        [allocations.sum(&:memsize).to_s, class_path]
      end
    end

    def objects_by_file
      allocs.group_by(&:path).map do |path, allocations|
        [allocations.size.to_s, path]
      end
    end

    def objects_by_location
      allocs.group_by { |a| "#{a.path}:#{a.line}" }.map do |location, allocations|
        [allocations.size.to_s, location]
      end
    end

    def objects_by_class
      allocs.group_by(&:object_class_path).map do |class_path, allocations|
        [allocations.size.to_s, class_path]
      end
    end

    def format_two_columns(data)
      return "" if data.empty?

      max_length = data.max_by { |row| row[0].size }[0].size
      data.map { |row| "#{row[0].ljust(max_length)} #{row[1]}" }.join("\n")
    end

    def allocs
      @allocs ||= allocations
    end
  end
end
